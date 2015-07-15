#include "Module.hpp"
#include "../Common/SimpleArchive.h"


// Additional structures for file I/O
#include <packon.h>

struct FileOffsetImportEntry
{
	char name[31];
	uint8_t importType;  // 0: PRT, 1 : ORT
	uint32_t appliedOffset;
};

struct FileOffsetExportEntry
{
	char name[31];
	char padding;
	uint32_t pointeeOffset;
};

#include <packoff.h>




// Main implementation.

ModulePtr deserializeModule(const std::string& moduleIdentifier, std::istream& is)
{
	ModulePtr module = std::make_shared<Module>();
	module->moduleIdentifier = moduleIdentifier;

	SimpleArchive arc(is);
	const std::vector<uint8_t>& data = arc.getSectionContent(STR2U32("data"));
	const std::vector<uint8_t>& prtData = arc.getSectionContent(STR2U32("prt "));
	const std::vector<uint8_t>& ortData = arc.getSectionContent(STR2U32("ort "));
	std::vector<uint8_t> importsData = arc.getSectionContent(STR2U32("impt"));
	std::vector<uint8_t> exportsData = arc.getSectionContent(STR2U32("expt"));

	// Read rawData
	module->rawData.data = std::move(data);
	size_t dataSize = module->rawData.data.size();
	size_t maxRelocData = (dataSize - 1) >> 2 << 2;  // biggest multiple of 4 less than dataSize


	// Read prt table
	{
		const uint32_t *prtArray = reinterpret_cast<const uint32_t*>(prtData.data());
		size_t prtn = prtData.size() >> 2;
		for(size_t i = 0; i < prtn; i++)
		{
			if(prtArray[i] & 3)
				fprintf(stderr, "[Module::Module] Warning 373 : Prt data #%d (%d) of module \"%s\" is not a multiple of 4. Will be ignored.\n",
				i + 1, prtArray[i], moduleIdentifier.c_str());

			else if(prtArray[i] > maxRelocData)
				fprintf(stderr, "[Module::Module] Warning 799 : Prt data #%d (%d) of module \"%s\" is too big ( > %d). Will be ignored\n",
				i + 1, prtArray[i], moduleIdentifier.c_str(), maxRelocData);

			else
				module->rawData.prt.push_back(prtArray[i]);
		}
	}


	// Read ort table
	{
		const uint32_t *ortArray = reinterpret_cast<const uint32_t*>(ortData.data());
		size_t ortn = ortData.size() >> 2;
		for(size_t i = 0; i < ortn; i++)
		{
			if(ortArray[i] & 3)
				fprintf(stderr, "[Module::Module] Warning 3 : Ort data #%d (%d) of module \"%s\" is not a multiple of 4. Will be ignored.\n",
				i + 1, ortArray[i], moduleIdentifier.c_str());

			else if(ortArray[i] > maxRelocData)
				fprintf(stderr, "[Module::Module] Warning 479 : Ort data #%d (%d) of module \"%s\" is too big ( > %d). Will be ignored\n",
				i + 1, ortArray[i], moduleIdentifier.c_str(), maxRelocData);

			else
				module->rawData.ort.push_back(ortArray[i]);
		}
	}


	// Read import table
	{
		const FileOffsetImportEntry *iteArray = reinterpret_cast<const FileOffsetImportEntry*>(importsData.data());
		size_t iten = importsData.size() / sizeof(FileOffsetImportEntry);
		for(size_t i = 0; i < iten; i++)
		{
			FileOffsetImportEntry ite = iteArray[i];
			ite.name[14] = '\0';

			if(ite.importType != IMPORT_PRT && ite.importType != IMPORT_ORT)
			{
				fprintf(stderr, "[Module::Module] Warning 962 : Import #%d (\"%s\", %06X) of module \"%s\" has invalid import type(%d). Will be ignored\n",
					i + 1, ite.name, ite.appliedOffset, moduleIdentifier.c_str(), ite.importType);
				continue;
			}

			else if(ite.appliedOffset & 3)
			{
				fprintf(stderr, "[Module::Module] Warning 874 : Import #%d (\"%s\", %06X) of module \"%s\" applies to non-4byte-aligned offset. Will be ignored\n",
					i + 1, ite.name, ite.appliedOffset, moduleIdentifier.c_str());
				continue;
			}

			module->imports.push_back({
				ite.importType == IMPORT_PRT ? IMPORT_PRT : IMPORT_ORT,
				ite.name,
				ite.appliedOffset
			});
		}
	}

	// Read export table
	{
		const FileOffsetExportEntry *eteArray = reinterpret_cast<const FileOffsetExportEntry*>(exportsData.data());
		size_t eten = exportsData.size() / sizeof(FileOffsetExportEntry);
		for(size_t i = 0; i < eten; i++)
		{
			FileOffsetExportEntry ete = eteArray[i];
			ete.name[14] = '\0';

			if(ete.pointeeOffset & 3)
			{
				fprintf(stderr, "[Module::Module] Warning 874 : Export #%d (\"%s\", %06X) of module \"%s\" points to non-4byte-aligned offset. Will be ignored\n",
					i + 1, ete.name, ete.pointeeOffset, moduleIdentifier.c_str());
				continue;
			}

			module->exports.push_back({
				ete.name,
				ete.pointeeOffset
			});
		}
	}

	return module;
}



std::vector<uint8_t> serializeModule(ModulePtr module)
{
	// Calculate size of total payload
	size_t payloadSize = 0;
	payloadSize += 8 + module->rawData.data.size();
	payloadSize += 8 + 4 * module->rawData.prt.size();
	payloadSize += 8 + 4 * module->rawData.ort.size();
	payloadSize += 8 + sizeof(FileOffsetImportEntry) * module->imports.size();
	payloadSize += 8 + sizeof(FileOffsetExportEntry) * module->exports.size();

	// Create payload buffer
	std::vector<uint8_t> output(payloadSize);
	uint8_t* p = output.data();

	// "data" section
	{
		uint32_t sectionSize = module->rawData.data.size();
		memcpy(p, "data", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, module->rawData.data.data(), sectionSize); p += sectionSize;
	}

	// "prt " section
	{
		uint32_t sectionSize = module->rawData.prt.size() * 4;
		memcpy(p, "prt ", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, module->rawData.prt.data(), sectionSize); p += sectionSize;
	}

	// "ort " section
	{
		uint32_t sectionSize = module->rawData.ort.size() * 4;
		memcpy(p, "ort ", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, module->rawData.ort.data(), sectionSize); p += sectionSize;
	}

	// "impt" section
	{
		uint32_t sectionSize = sizeof(FileOffsetImportEntry) * module->imports.size();
		memcpy(p, "impt", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		for(const auto& ite : module->imports)
		{
			FileOffsetImportEntry fite = { 0 };
			strncpy(fite.name, ite.importName.c_str(), 30);
			fite.appliedOffset = ite.appliedOffset;
			fite.importType = ite.importType;
			memcpy(p, &fite, sizeof(FileOffsetImportEntry));
			p += sizeof(FileOffsetImportEntry);
		}
	}

	// "expt" section
	{
		uint32_t sectionSize = sizeof(FileOffsetExportEntry) * module->exports.size();
		memcpy(p, "expt", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		for(const auto& ete : module->exports)
		{
			FileOffsetExportEntry fete = { 0 };
			strncpy(fete.name, ete.exportName.c_str(), 30);
			fete.pointeeOffset = ete.pointeeOffset;
			memcpy(p, &fete, sizeof(FileOffsetExportEntry));
			p += sizeof(FileOffsetExportEntry);
		}
	}

	return output;
}
