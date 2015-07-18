#include "Module.hpp"
#include "SimpleArchive.hpp"


// Internal implementation
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

Module::Module(const std::string& moduleIdentifier, std::istream& is)
	: moduleIdentifier(moduleIdentifier)
{
	bytes data, prtData, ortData, importsData, exportsData;

	{
		SimpleArchive arc(is);
		if(!(
			arc.getSectionContent('atad', data) &&
			arc.getSectionContent(' trp', prtData) &&
			arc.getSectionContent(' tro', ortData) &&
			arc.getSectionContent('tpmi', importsData) &&
			arc.getSectionContent('tpxe', exportsData)
			))
		{
			fprintf(stderr, "Linker warning 539 : Module may be corrupted: one or more sections missing.");
			throw std::bad_exception("Bad section");
		}
	}

	// Read rawData
	rawData.data = std::move(data);
	size_t dataSize = rawData.data.size();
	size_t maxRelocData = (dataSize - 1) >> 2 << 2;  // biggest multiple of 4 less than dataSize


	// Read prt table
	{
		const uint32_t *prtArray = reinterpret_cast<const uint32_t*>(prtData.data());
		size_t prtn = prtData.size() >> 2;
		for(size_t i = 0; i < prtn; i++)
		{
			if(prtArray[i] & 3)
				fprintf(stderr, "Linker warning 373 : Prt data #%d (%d) of module \"%s\" is not a multiple of 4. Will be ignored.\n",
				i + 1, prtArray[i], moduleIdentifier.c_str());

			else if(prtArray[i] > maxRelocData)
				fprintf(stderr, "Linker warning 799 : Prt data #%d (%d) of module \"%s\" is too big ( > %d). Will be ignored\n",
				i + 1, prtArray[i], moduleIdentifier.c_str(), maxRelocData);

			else
				rawData.prt.push_back(prtArray[i]);
		}
	}


	// Read ort table
	{
		const uint32_t *ortArray = reinterpret_cast<const uint32_t*>(ortData.data());
		size_t ortn = ortData.size() >> 2;
		for(size_t i = 0; i < ortn; i++)
		{
			if(ortArray[i] & 3)
				fprintf(stderr, "Linker warning 3 : Ort data #%d (%d) of module \"%s\" is not a multiple of 4. Will be ignored.\n",
				i + 1, ortArray[i], moduleIdentifier.c_str());

			else if(ortArray[i] > maxRelocData)
				fprintf(stderr, "Linker warning 479 : Ort data #%d (%d) of module \"%s\" is too big ( > %d). Will be ignored\n",
				i + 1, ortArray[i], moduleIdentifier.c_str(), maxRelocData);

			else
				rawData.ort.push_back(ortArray[i]);
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
				fprintf(stderr, "Linker warning 962 : Import #%d (\"%s\", %06X) of module \"%s\" has invalid import type(%d). Will be ignored\n",
					i + 1, ite.name, ite.appliedOffset, moduleIdentifier.c_str(), ite.importType);
				continue;
			}

			else if(ite.appliedOffset & 3)
			{
				fprintf(stderr, "Linker warning 874 : Import #%d (\"%s\", %06X) of module \"%s\" applies to non-4byte-aligned offset. Will be ignored\n",
					i + 1, ite.name, ite.appliedOffset, moduleIdentifier.c_str());
				continue;
			}

			Imports.push_back({
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
				fprintf(stderr, "Linker warning 874 : Export #%d (\"%s\", %06X) of module \"%s\" points to non-4byte-aligned offset. Will be ignored\n",
					i + 1, ete.name, ete.pointeeOffset, moduleIdentifier.c_str());
				continue;
			}

			Exports.push_back({
				ete.name,
				ete.pointeeOffset
			});
		}
	}
}


bytes Module::toBytes() const
{
	// Calculate size of total payload
	size_t payloadSize = 0;
	payloadSize += 8 + rawData.data.size();
	payloadSize += 8 + 4 * rawData.prt.size();
	payloadSize += 8 + 4 * rawData.ort.size();
	payloadSize += 8 + sizeof(FileOffsetImportEntry) * Imports.size();
	payloadSize += 8 + sizeof(FileOffsetExportEntry) * Exports.size();

	// Create payload buffer
	bytes output(payloadSize);
	uint8_t* p = output.data();

	// "data" section
	{
		uint32_t sectionSize = rawData.data.size();
		memcpy(p, "data", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, rawData.data.data(), sectionSize); p += sectionSize;
	}

	// "prt " section
	{
		uint32_t sectionSize = rawData.prt.size() * 4;
		memcpy(p, "prt ", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, rawData.prt.data(), sectionSize); p += sectionSize;
	}

	// "ort " section
	{
		uint32_t sectionSize = rawData.ort.size() * 4;
		memcpy(p, "ort ", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		memcpy(p, rawData.ort.data(), sectionSize); p += sectionSize;
	}

	// "impt" section
	{
		uint32_t sectionSize = sizeof(FileOffsetImportEntry) * Imports.size();
		memcpy(p, "impt", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		for(const auto& ite : Imports)
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
		uint32_t sectionSize = sizeof(FileOffsetExportEntry) * Imports.size();
		memcpy(p, "expt", 4); p += 4;
		memcpy(p, &sectionSize, 4); p += 4;
		for(const auto& ete : Exports)
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
