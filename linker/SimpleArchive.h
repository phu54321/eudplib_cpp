#pragma once

#include <cstdint>

#include <vector>
#include <istream>
#include <map>

#define STR2U32(s) (*(uint32_t*)(s))

class SimpleArchive
{
public:
	SimpleArchive(std::istream& is);
	~SimpleArchive();

	std::vector<uint8_t> getSectionContent(uint32_t sectionName) const;

private:
	std::map<uint32_t, std::vector<uint8_t>> contentMap;
};
