#include "ModuleWriterCollector.hpp"
#include "Expr.hpp";
#include "EUDObject.hpp"

#include <stack>

ModuleWriterCollector::ModuleWriterCollector(EUDObject* root)
{
	_remainingObjects.push(root);

	while(!_remainingObjects.empty())
	{
		EUDObject* obj = _remainingObjects.top();
		_remainingObjects.pop();
		if(_objList.find(obj) != _objList.end()) continue;  // Duplicate

		root->writePayload(this);
	}
}

ModuleWriterCollector::~ModuleWriterCollector() {}


///////


void ModuleWriterCollector::writeByte(uint8_t) {}
void ModuleWriterCollector::writeWord(uint16_t) {}
void ModuleWriterCollector::writeDword(uint32_t) {}
void ModuleWriterCollector::writeDword(const Expr& expr)
{
	if(expr.getTargetType() == TARGET_OBJECT)
	{
		_remainingObjects.push(expr.getTargetObject());
	}
}

std::set<EUDObject*> ModuleWriterCollector::getCollectedObjects() const
{
	return _objList;
}
