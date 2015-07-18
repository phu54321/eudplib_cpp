#pragma once

#include <string>
#include <memory>
#include <cstdint>

class EUDObject;

enum TargetType
{
	TARGET_NONE,
	TARGET_OBJECT,
	TARGET_NAME,
	TARGET_FORWARD,
};

// Object of this class is immutable.
class Forward;

class Expr
{
public:
	Expr(EUDObject* obj, bool div4, uint32_t offset);
	Expr(const std::string& str, bool div4, uint32_t offset);
	Expr(const Forward& obj, bool div4 = false, uint32_t offset = 0);
	Expr(uint32_t offset);
	virtual ~Expr();

	Expr operator+(int rhs) const;
	Expr operator-(int rhs) const;
	Expr operator/(int rhs) const;

	TargetType getTargetType();
	EUDObject* getTargetObject();
	std::string getTargetName();
	bool isTargetDividedBy4();
	uint32_t getOffset();

private:
	void resolveForward();

	TargetType _targetType;
	union TargetCollection
	{
		TargetCollection() : object(nullptr) {}
		TargetCollection(EUDObject* obj) : object(obj) {}
		TargetCollection(Forward* fw) : forward(fw) {}
		TargetCollection(const std::string& name) : name(new std::string(name)) {}
		EUDObject* object;
		Forward* forward;
		std::string* name;
	} _target;
	bool _div4;

	uint32_t _offset;

};

Expr operator+(int lhs, const Expr& rhs);
Expr getNameExpr(const std::string& name);
