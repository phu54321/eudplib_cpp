#pragma once

#include <vector>
#include <cstdint>
#include <memory>

struct Payload
{
	std::vector<uint8_t> data;
	std::vector<size_t> prt;
	std::vector<size_t> ort;

	size_t size() const { return data.size(); }
};

typedef std::shared_ptr<Payload> PayloadPtr;
