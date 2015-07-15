#include "SimpleArchive.hpp"

SimpleArchive::SimpleArchive() {}

SimpleArchive::SimpleArchive(std::istream& is)
{
	while(true)
	{
		uint32_t sectionName, sectionSize;
		is.read((char*)&sectionName, 4);
		is.read((char*)&sectionSize, 4);
		if(is.eof()) break;

		bytes data(sectionSize);
		is.read((char*)data.data(), sectionSize);
		if(is.eof()) break;

		contentMap[sectionName] = std::move(data);
	}
}

SimpleArchive::~SimpleArchive() {}

bool SimpleArchive::getSectionContent(uint32_t sectionName, bytes& result) const
{
	auto it = contentMap.find(sectionName);
	if(it == contentMap.end()) return false;
	else
	{
		result = it->second;
		return true;
	}
}

void SimpleArchive::setSectionContent(uint32_t sectionName, const bytes& src)
{
	contentMap[sectionName] = src;
}

bytes SimpleArchive::toBytes() const
{
	size_t payloadSize = 0;
	for(const auto& it : contentMap)
	{
		payloadSize += 8 + it.second.size();
	}

	bytes payload(payloadSize);
	uint8_t* p = payload.data();
	for(const auto& it : contentMap)
	{
		uint32_t sectionName = it.first, sectionSize = it.second.size();
		memcpy(p + 0, &sectionName, 4);
		memcpy(p + 4, &sectionSize, 4);
		memcpy(p + 8, it.second.data(), sectionSize);
		p += 8 + sectionSize;
	}

	return payload;
}