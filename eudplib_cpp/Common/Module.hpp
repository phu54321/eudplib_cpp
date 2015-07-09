#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Payload.hpp"

// Linker links multiple modules
/*

Linker links multiple modules. Each modules have following items
 - Data
 - PRT (Player Relocation Table)
 - ORT (Offset Relocation Table)
 - OT  (Offset Table)            : Export offsets
*/



// Internal types

enum ImportType
{
	IMPORT_PRT = 1,
	IMPORT_ORT = 4,
};

struct OffsetImportEntry
{
	ImportType importType;
	std::string importName;
	size_t appliedOffset;
};

struct OffsetExportEntry
{
	std::string exportName;
	size_t pointeeOffset;
};

struct Module
{
	std::string moduleIdentifier;

	Payload rawData;
	std::vector<OffsetImportEntry> imports;
	std::vector<OffsetExportEntry> exports;
};

typedef std::shared_ptr<Module> ModulePtr;


// Serializer & Deserializer
ModulePtr deserializeModule(const std::string& moduleIdentifier, std::istream& is);
std::vector<uint8_t> serializeModule(ModulePtr module);
