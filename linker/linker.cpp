#include "Module.hpp"

#include <cstdio>

#include <unordered_map>
#include <string>
#include <stack>
#include <set>

struct LinkerState
{
	bool hasError;
	std::vector<ModulePtr> modules;
	std::unordered_map<std::string, std::pair<ModulePtr, size_t>> labelMap;
};


///////


bool buildLabelMap(LinkerState& ls)
{
	ls.labelMap.clear();

	for(const auto& module : ls.modules)
	{
		for(const auto& exportEntry : module->Exports)
		{
			auto it = ls.labelMap.find(exportEntry.exportName);
			if(it != ls.labelMap.end())
			{
				ModulePtr prevModule = it->second.first;
				fprintf(stderr, "[buildLabelMap] Error 5240 : Duplicate name \"%s\" exported from \"%s\" and \"%s\".\n",
					exportEntry.exportName.c_str(), module->moduleIdentifier.c_str(), prevModule->moduleIdentifier.c_str());
				ls.hasError = true;
			}
			else ls.labelMap[exportEntry.exportName] = std::make_pair(module, exportEntry.pointeeOffset);
		}
	}

	return !ls.hasError;
}



bool filterRequiredModules(LinkerState& ls)
{
	const auto& labelMap = ls.labelMap;
	const auto& modules = ls.modules;

	if(modules.empty())
	{
		fprintf(stderr, "[filterRequiredModules] Error 7911 : No root module found.\n");
	}

	std::stack<ModulePtr> remainingModules;
	std::set<ModulePtr> collectedModules;
	std::vector<ModulePtr> newModuleVector;

	remainingModules.push(modules[0]);  // Seed

	while(!remainingModules.empty())
	{
		auto module = remainingModules.top(); remainingModules.pop();
		if(collectedModules.find(module) != collectedModules.end()) continue;  // Module already collected

		// Note : since modules[0] is the first one to be checked,
		// newModuleVector[0] should be the same pointer as modules[0].
		collectedModules.insert(module);
		newModuleVector.push_back(module);

		for(const auto& importEntry : module->Imports)
		{
			auto it = labelMap.find(importEntry.importName);
			if(it == labelMap.end())
			{
				fprintf(stderr, "[filterRequiredModules] Error 2563 : Unresolved name \"%s\" in \"%s\".\n",
					importEntry.importName.c_str(), module->moduleIdentifier.c_str());
				ls.hasError = true;
			}
			else remainingModules.push(it->second.first);
		}
	}

	// Optimize module vector;
	if(ls.hasError) return false;
	ls.modules = newModuleVector;
	return true;
}



PayloadPtr createPayload(LinkerState& ls)
{
	PayloadPtr payload = std::make_shared<Payload>();
	
	// Allocate each modules
	std::unordered_map<ModulePtr, size_t> moduleOffsetMap;
	size_t currentOffset = 0;
	for(const auto& module : ls.modules)
	{
		moduleOffsetMap[module] = currentOffset;
		// Round up to 4byte boundary : Each module's starting offset should be multiple of 4. (EUD works on dwords)
		currentOffset += (module->rawData.size() + 3) & ~3;
	}
	// now currentOffset is the total size of payload.
	payload->data.resize(currentOffset);

	// Build payload
	uint8_t* payloadData = payload->data.data();
	for(auto& module : ls.modules)
	{
		uint8_t* moduleData = module->rawData.data.data();
		size_t moduleOffset = moduleOffsetMap[module];
		size_t moduleSize = module->rawData.size();

		// Copy raw data
		memcpy(payloadData + moduleOffset, moduleData, moduleSize);

		// Apply import table
		for(const auto& importEntry : module->Imports)
		{
			std::pair<ModulePtr, size_t> pointee = ls.labelMap[importEntry.importName];
			size_t pointeeOffset = moduleOffsetMap[pointee.first] + pointee.second;
			size_t appendValue = (importEntry.importType == IMPORT_PRT) ? pointeeOffset >> 2 : pointeeOffset;
			*(uint32_t*)(payloadData + moduleOffset + importEntry.appliedOffset) += appendValue;
		}

		// Append prt, ort
		for(size_t prtOffset : module->rawData.prt) payload->prt.push_back(prtOffset + moduleOffset);
		for(size_t ortOffset : module->rawData.prt) payload->ort.push_back(ortOffset + moduleOffset);
	}

	return payload;
}



PayloadPtr linkModules(const std::vector<ModulePtr>& modules)
{
	LinkerState ls = { false, modules };
	if(!buildLabelMap(ls)) return nullptr;
	if(!filterRequiredModules(ls)) return nullptr;
	return createPayload(ls);
}
