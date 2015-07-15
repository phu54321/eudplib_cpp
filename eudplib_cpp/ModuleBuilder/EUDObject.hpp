#pragma once

#include <memory>

class ModuleBuilder;

class EUDObject
{
public:
	EUDObject();
	virtual ~EUDObject();

	size_t getObjectSize() const;
	void writeObject(ModuleBuilder& builder) const;
};
