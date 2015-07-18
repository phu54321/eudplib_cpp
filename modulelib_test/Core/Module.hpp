#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Payload.hpp"
#include "../typedef.hpp"

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
	// Note : constructor and convertToBytes should be used exclusively.
	// - Linker must only use constructors.
	// - Compiler must only use convertToBytes.
	//
	// This limitation is due to 31byte character limits of import/export name.
	// Length of std::string is not limited, but only first 31bytes are written into module files.
	//
	// Linker only should work with constructor-constructed  
	Module(const std::string& moduleIdentifier) : moduleIdentifier(moduleIdentifier) {}
	Module(const std::string& moduleIdentifier, std::istream& is);
	bytes toBytes() const;


	std::string moduleIdentifier;

	Payload rawData;
	std::vector<OffsetImportEntry> Imports;
	std::vector<OffsetExportEntry> Exports;
};

typedef std::shared_ptr<Module> ModulePtr;

