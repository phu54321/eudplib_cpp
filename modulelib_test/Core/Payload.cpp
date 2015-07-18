#include "Payload.hpp"
#include "SimpleArchive.hpp"

Payload::Payload() {}
Payload::Payload(std::istream& is)
{
	SimpleArchive arc(is);
	bytes prtData, ortData;
	arc.getSectionContent('atad', data);
	arc.getSectionContent(' trp', prtData);
	arc.getSectionContent(' tro', ortData);

	size_t prtN = prtData.size() / 4, ortN = ortData.size() / 4;
	prt.resize(prtN);
	ort.resize(ortN);
	memcpy(prt.data(), prtData.data(), prtN * 4);
	memcpy(ort.data(), ortData.data(), ortN * 4);
}


bytes Payload::toBytes() const
{
	SimpleArchive arc;
	arc.setSectionContent('atad', data);

	{
		bytes prtData(prt.size() * 4);
		memcpy(prtData.data(), prt.data(), prt.size() * 4);
		arc.setSectionContent(' trp', prtData);
	}

	{
		bytes ortData(ort.size() * 4);
		memcpy(ortData.data(), ort.data(), ort.size() * 4);
		arc.setSectionContent(' tro', ortData);
	}

	return arc.toBytes();
}
