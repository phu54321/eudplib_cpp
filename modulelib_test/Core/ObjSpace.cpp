#include "ObjSpace.hpp"
#include "EUDObject.hpp"

typedef int* a;
typedef int* a;

ObjSpace::ObjSpace() {}
ObjSpace::~ObjSpace()
{
	for(EUDObject* o : _objv)
	{
		delete o;
	}
}

void ObjSpace::push(EUDObject* o)
{
	_objv.push_back(o);
}
