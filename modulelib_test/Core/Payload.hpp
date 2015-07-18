#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <istream>

#include "../typedef.hpp"

struct Payload
{
	Payload();
	Payload(std::istream& is);
	bytes toBytes() const;

	bytes data;
	std::vector<size_t> prt;
	std::vector<size_t> ort;
};

typedef std::shared_ptr<Payload> PayloadPtr;
