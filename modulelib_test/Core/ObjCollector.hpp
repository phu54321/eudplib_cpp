#pragma once

#include "PayloadWriter.hpp"
#include <set>
#include <stack>

class EUDObject;

class ObjCollector : public PayloadWriter
{
public:
	ObjCollector();
	virtual ~ObjCollector();

	virtual void writeByte(uint8_t ch);
	virtual void writeWord(uint16_t i);
	virtual void writeDword(uint32_t i);
	virtual void writeDword(const Expr& expr);
	virtual void writeBytes(const void* data, size_t len);
	virtual void writeBytes(const bytes& b);

	std::set<EUDObject*> collectObjects(EUDObject* root);

private:
	std::stack<EUDObject*> *_remainingObjects;
};
