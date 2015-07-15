#pragma once

#include "Expr.hpp"
#include "ObjSpace.hpp"
#include <cstdint>

class ModuleWriter;

class EUDObject : public Expr
{
public:
	EUDObject(ObjSpace& objs) : Expr(this, false, 0) { objs.push(this); }
	virtual ~EUDObject() {}

	virtual uint32_t getObjectSize() const = 0;
	virtual void writePayload(ModuleWriter* objw) = 0;
};
