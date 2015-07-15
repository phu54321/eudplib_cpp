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
};



// Object of this class is immutable.
class Expr
{

public:
	Expr(EUDObject* obj, bool div4, uint32_t offset = 0);
	Expr(const std::string& str, bool div4, uint32_t offset = 0);
	Expr(uint32_t offset);
	virtual ~Expr();

	Expr operator+(int rhs) const;
	Expr operator-(int rhs) const;
	Expr operator/(int rhs) const;


	TargetType getTargetType() const;
	EUDObject* getTargetObject() const;
	std::string getTargetName() const;
	bool isTargetDividedBy4() const;
	uint32_t getOffset() const;

private:
	const bool _usetarget;
	const EUDObject* _targetobj;
	const std::string _targetname;
	bool _div4;

	uint32_t _offset;
};

Expr operator+(int lhs, const Expr& rhs);

