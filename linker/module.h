#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "payload.h"

// Linker links multiple modules
/*

Linker links multiple modules. Each modules have following items
 - Data
 - PRT (Player Relocation Table)
 - ORT (Offset Relocation Table)
 - OT  (Offset Table)            : Export offsets

*/

enum ImportType
{
	IMPORT_PRT,
	IMPORT_ORT,
};

struct offsetImportEntry
{
	ImportType importType;
	std::string importName;
	size_t appliedPosition;
};

struct offsetExportEntry
{
	std::string exportName;
	size_t pointingPosition;
};

struct Module
{
	std::string moduleIdentifier;

	Payload rawData;
	std::vector<offsetImportEntry> Imports;
	std::vector<offsetExportEntry> Exports;
};

typedef std::shared_ptr<Module> ModulePtr;
