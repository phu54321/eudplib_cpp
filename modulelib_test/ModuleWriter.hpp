#pragma once

#include <cstdint>

class Expr;

class ModuleWriter
{
public:
	ModuleWriter() {}
	virtual ~ModuleWriter() {}

	virtual void writeByte(uint8_t ch) = 0;
	virtual void writeWord(uint16_t i) = 0;
	virtual void writeDword(uint32_t i) = 0;
	virtual void writeDword(const Expr& expr) = 0;
};
