// Use in conjunction with interactive debugger

#include "Module.hpp"

#include <sstream>
#include <cassert>

char data[] = {
	// rawdata section
	'd', 'a', 't', 'a',
	0x07, 0x00, 0x00, 0x00,  // Size = 7
	0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,

	// prt section
	'p', 'r', 't', ' ',
	0x0C, 0x00, 0x00, 0x00,  // Size = 12
	/* 0 */ 0x00, 0x00, 0x00, 0x00,
	/* 4 */ 0x04, 0x00, 0x00, 0x00,  // OOB
	/* 6 */ 0x06, 0x00, 0x00, 0x00,  // Not 4byte-aligned

	// ort section
	'o', 'r', 't', ' ',
	0x05, 0x00, 0x00, 0x00,  // Size = 5  (Test for non-4bye-aligned table)
	/* 4 */ 0x04, 0x00, 0x00, 0x00,
	0x00,  // padding

	// import section
	'i', 'm', 'p', 't',
	0x24, 0x00, 0x00, 0x00,  // Size = 36,
	'a', 'd', 'd', 'p', 'l', 'a', 'c', 'e', 'r', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	IMPORT_PRT,
	0x04, 0x00, 0x00, 0x00,

	// Export section
	'e', 'x', 'p', 't',
	0x24, 0x00, 0x00, 0x00,  // Size = 36,
	'b', 'e', 'i', 'n', 'g', 'e', 'x', 'p', 'o', 'r', 't', 'e', 'd', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,  // padding
	0x00, 0x00, 0x00, 0x00,
};


int main()
{
	std::string s(data, data + sizeof(data));
	std::istringstream ss(s);

	Module m("test module", ss);
	std::vector<uint8_t> v = m.convertToBytes();
	return 0;
}