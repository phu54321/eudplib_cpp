#pragma once

#include "ModuleWriter.hpp"
#include <set>
#include <stack>

class EUDObject;

class ModuleWriterCollector : public ModuleWriter
{
public:
	ModuleWriterCollector(EUDObject* root);
	virtual ~ModuleWriterCollector();

	virtual void writeByte(uint8_t ch);
	virtual void writeWord(uint16_t i);
	virtual void writeDword(uint32_t i);
	virtual void writeDword(const Expr& expr);

	std::set<EUDObject*> getCollectedObjects() const;

private:
	std::stack<EUDObject*> _remainingObjects;
	std::set<EUDObject*> _objList;
};
