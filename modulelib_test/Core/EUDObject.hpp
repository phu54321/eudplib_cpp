#pragma once

#include "Expr.hpp"
#include "ObjSpace.hpp"
#include <cstdint>

class ObjWriter;

class EUDObject : public Expr
{
public:
	EUDObject(ObjSpace& objs) : Expr(this, false, 0) { objs.push(this); }
	virtual ~EUDObject() {}

	virtual uint32_t getObjectSize() const = 0;
	virtual void writePayload(ObjWriter* pw) = 0;
};
