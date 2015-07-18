#pragma once

#include <cstdint>
#include "../typedef.hpp"

class Expr;

class PayloadWriter
{
public:
	PayloadWriter() {}
	virtual ~PayloadWriter() {}

	virtual void writeByte(uint8_t ch) = 0;
	virtual void writeWord(uint16_t i) = 0;
	virtual void writeDword(uint32_t i) = 0;
	virtual void writeDword(const Expr& expr) = 0;
	virtual void writeBytes(const void* data, size_t len) = 0;
	virtual void writeBytes(const bytes& b) = 0;
};
