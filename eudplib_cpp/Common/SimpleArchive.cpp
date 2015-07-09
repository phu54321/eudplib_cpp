#include "SimpleArchive.h"

SimpleArchive::SimpleArchive(std::istream& is)
{
	while(true)
	{
		uint32_t sectionName, sectionSize;
		is.read((char*)&sectionName, 4);
		is.read((char*)&sectionSize, 4);
		if(is.eof()) break;

		std::vector<uint8_t> data(sectionSize);
		is.read((char*)data.data(), sectionSize);
		if(is.eof()) break;

		contentMap[sectionName] = std::move(data);
	}
}

SimpleArchive::~SimpleArchive() {}

std::vector<uint8_t> SimpleArchive::getSectionContent(uint32_t sectionName) const
{
	auto it = contentMap.find(sectionName);
	if(it == contentMap.end()) return std::vector<uint8_t>();
	else return contentMap.find(sectionName)->second;
}
