#pragma once

#include <cstdint>
#include <vector>
#include <istream>
#include <map>

#include "../typedef.hpp"


class SimpleArchive
{
public:
	SimpleArchive();
	SimpleArchive(std::istream& is);
	~SimpleArchive();

	bool getSectionContent(uint32_t sectionName, bytes& result) const;
	void setSectionContent(uint32_t sectionName, const bytes& src);
	bytes toBytes() const;

private:
	std::map<uint32_t, bytes> _contentMap;
};
