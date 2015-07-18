#include "ModuleWriter.hpp"
#include "ObjCollector.hpp"
#include "EUDObject.hpp"
#include "Module.hpp"

#include <cassert>
#include <unordered_map>

struct ModuleWriterState
{
	std::unordered_map<EUDObject*, uint32_t> offsetMap;
	ModulePtr currentModule;
	uint32_t currentOffset;
};

ModuleWriter::ModuleWriter() {}
ModuleWriter::~ModuleWriter() {}

ModulePtr ModuleWriter::createModule(const std::string& moduleIdentifier, EUDObject* root)
{
	// Collect objects
	std::vector<EUDObject*> objv = { root };
	{
		auto objs = ObjCollector().collectObjects(root);
		objs.erase(root);
		for(auto obj : objs) objv.push_back(obj);
	}

	// Allocate objects.
	std::vector<uint32_t> objOffsetv;
	uint32_t payloadSize;
	{
		uint32_t currentOffset = 0;
		for(auto obj : objv)
		{
			objOffsetv.push_back(currentOffset);
			currentOffset += (obj->getObjectSize() + 3) & ~3;
		}
		payloadSize = currentOffset;
	}

	// Create module
	ModulePtr module = std::make_shared<Module>(moduleIdentifier);
	module->rawData.data.resize(payloadSize);

	std::shared_ptr<ModuleWriterState> state(new ModuleWriterState);
	state->currentModule = module;
	state->currentOffset = 0;

	// Write allocation data.
	{
		for(size_t i = 0; i < objv.size(); i++)
		{
			EUDObject* obj = objv[i];
			size_t offset = objOffsetv[i];
			state->offsetMap[obj] = offset;
		}
	}


	// Write objects
	_state = state.get();
	{
		for(auto obj : objv)
		{
			assert(state->currentOffset == objOffsetv);
			obj->writePayload(this);
			state->currentOffset = (state->currentOffset + 3) & ~3;
		}
		assert(state->currentOffset == payloadSize);
	}

	// Done!
	return module;
}


void ModuleWriter::writeByte(uint8_t ch)
{
	auto& outbuf = _state->currentModule->rawData.data;
	uint8_t* outData = outbuf.data();
	size_t payloadSize = outbuf.size();
	assert(payloadSize >= _state->currentOffset + 1);
	*(outData + _state->currentOffset) = ch;
	_state->currentOffset += 1;
}


void ModuleWriter::writeWord(uint16_t i)
{
	assert(_state->currentOffset % 2 == 0);
	auto& outbuf = _state->currentModule->rawData.data;
	uint8_t* outData = outbuf.data();
	size_t payloadSize = outbuf.size();
	assert(payloadSize >= _state->currentOffset + 2);
	*(uint16_t*)(outData + _state->currentOffset) = i;
	_state->currentOffset += 2;
}


void ModuleWriter::writeDword(uint32_t i)
{
	assert(_state->currentOffset % 4 == 0);
	auto& outbuf = _state->currentModule->rawData.data;
	uint8_t* outData = outbuf.data();
	size_t payloadSize = outbuf.size();
	assert(payloadSize >= _state->currentOffset + 4);
	*(uint32_t*)(outData + _state->currentOffset) = i;
	_state->currentOffset += 4;
}


void ModuleWriter::writeDword(const Expr& exp)
{
	assert(_state->currentOffset % 4 == 0);

	// writeDword(uint32_t) adds _state->currentOffset by 4.
	// so calling it should be after altering relocation tables.

	auto targetType = exp.getTargetType();
	switch(targetType)
	{
	case TARGET_NONE:
		writeDword((uint32_t)exp.getOffset());  // Just write it.
		break;

	case TARGET_OBJECT:
	{
		EUDObject* target = exp.getTargetObject();
		auto it = _state->offsetMap.find(target);
		assert(it != _state->offsetMap.end());
		uint32_t targetOffset = it->second;

		if(exp.isTargetDividedBy4())
		{
			_state->currentModule->rawData.prt.push_back(_state->currentOffset);
			writeDword((uint32_t)exp.getOffset() + targetOffset / 4);
		}
		else
		{
			_state->currentModule->rawData.ort.push_back(_state->currentOffset);
			writeDword((uint32_t)exp.getOffset() + targetOffset);
		}
		break;
	}

	case TARGET_NAME:
	{
		std::string targetName = exp.getTargetName();
		if(exp.isTargetDividedBy4())
		{
			_state->currentModule->Imports.push_back({ IMPORT_PRT, targetName, _state->currentOffset });
		}
		else
		{
			_state->currentModule->Imports.push_back({ IMPORT_ORT, targetName, _state->currentOffset });
		}
		writeDword((uint32_t)exp.getOffset());
		break;
	}
	}
}


void ModuleWriter::writeBytes(const void* data, size_t len)
{
	auto& outbuf = _state->currentModule->rawData.data;
	uint8_t* outData = outbuf.data();
	size_t payloadSize = outbuf.size();
	assert(payloadSize >= _state->currentOffset + len);
	memcpy(outData + _state->currentOffset, data, len);
	_state->currentOffset += len;
}


void ModuleWriter::writeBytes(const bytes& b)
{
	writeBytes(b.data(), b.size());
}
