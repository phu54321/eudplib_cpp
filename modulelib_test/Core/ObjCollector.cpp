#include "ObjCollector.hpp"
#include "Expr.hpp"
#include "EUDObject.hpp"

#include <stack>

ObjCollector::ObjCollector() {}
ObjCollector::~ObjCollector() {}


///////


void ObjCollector::writeByte(uint8_t) {}
void ObjCollector::writeWord(uint16_t) {}
void ObjCollector::writeDword(uint32_t) {}
void ObjCollector::writeDword(const Expr& expr)
{
	if(expr.getTargetType() == TARGET_OBJECT)
	{
		_remainingObjects->push(expr.getTargetObject());
	}
}

void ObjCollector::writeBytes(const void* data, size_t len) {}
void ObjCollector::writeBytes(const bytes& b) {}

std::set<EUDObject*> ObjCollector::collectObjects(EUDObject* root)
{
	std::set<EUDObject*> objList;

	std::shared_ptr<std::stack<EUDObject*>> remainingObjects(new std::stack<EUDObject*>());
	_remainingObjects = remainingObjects.get();

	remainingObjects->push(root);

	while(!remainingObjects->empty())
	{
		EUDObject* obj = remainingObjects->top();
		remainingObjects->pop();
		if(objList.find(obj) != objList.end()) continue;  // Duplicate

		root->writePayload(this);
	}

	///

	return objList;
}
