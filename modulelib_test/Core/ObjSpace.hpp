#pragma once

#include <vector>

class EUDObject;

// GC-like solution. Every object will be counted in objspace, and will be destroyed upon objspace's destruction.
// This can greatily simplify object destruction process without using fance Bohem's gc collector.
// ( I failed compiling bohem's gc. bb )
class ObjSpace
{
public:
	ObjSpace();
	~ObjSpace();

	void push(EUDObject* obj);

private:
	std::vector<EUDObject*> _objv;
};
