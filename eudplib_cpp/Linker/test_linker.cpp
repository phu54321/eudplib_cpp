#include "Linker.hpp"

PayloadPtr linkModules(const std::vector<ModulePtr>& modules);

void hexDump(const std::vector<uint8_t>& data)
{
	for(uint8_t ch : data)
	{
		printf("%02X ", ch);
	}
	printf("\n");
}

int main()
{
	std::vector<ModulePtr> modules = {
		// Module 1
		ModulePtr(new Module({
			"module1",  // Name
			{  // Payload
				{ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC },
				{ 4 },
				{ 0 },
			},
			{  // Imports
				{ IMPORT_ORT, "module3_export1", 4 }
			},
			{  // Exports
				{ "module1_export1", 0 },
				{ "module1_export2", 4 },
			}
		})),

		// Module 2  // unused
		ModulePtr(new Module({
			"module2",  // Name
			{  // Payload
				{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, },
				{ 4 },
				{},
			},
			{  // Imports
				{ IMPORT_PRT, "module1_export1", 4 }
			},
			{  // Exports
				{ "module2_export1", 0 },
				{ "module2_export2", 4 },
			}
		})),

		// Module 3
		ModulePtr(new Module({
			"module2",  // Name
			{  // Payload
				{ 0x22, 0x22, 0x22, 0x22, 0x22, },  // 5byte. should truncate to 8bytes
				{},
				{},
			},
			{  // Imports
				{ IMPORT_PRT, "module1_export1", 0 },
				{ IMPORT_PRT, "module1_export2", 4 },
			},
			{  // Exports
				{ "module3_export1", 0 },
			}
		}))
	};

	PayloadPtr payload = linkModules(modules);
	hexDump(payload->data);
	system("pause");

	return 0;
}
