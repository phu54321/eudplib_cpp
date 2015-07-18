#include "Expr.hpp"
#include "Forward.hpp"
#include <cassert>

Expr::Expr(EUDObject* obj, bool div4, uint32_t offset)
	: _targetType(TARGET_OBJECT), _target(obj), _div4(div4), _offset(offset)
{}

Expr::Expr(const std::string& str, bool div4, uint32_t offset)
	: _targetType(TARGET_NAME), _target(str),  _div4(div4), _offset(offset)
{}

Expr::Expr(const Forward& fw, bool div4, uint32_t offset)
	: _targetType(TARGET_FORWARD), _target(fw), _div4(div4), _offset(offset)
{}

Expr::Expr(uint32_t offset)
	: _targetType(TARGET_NONE), _div4(false), _offset(offset)
{}

Expr::~Expr() {
	if(_targetType == TARGET_NAME) delete _target.name;
}

////////

Expr Expr::operator+(int rhs) const
{
	Expr t = *this;
	t._offset += rhs;
	return t;
}

Expr Expr::operator-(int rhs) const
{
	Expr t = *this;
	t._offset -= rhs;
	return t;
}

Expr Expr::operator/(int rhs) const
{
	if(rhs == 1) return *this;
	else if(rhs == 4)
	{
		assert(_targetType == TARGET_NONE || !div4);
		assert(offset % 4 == 0);
		
		Expr t = *this;
		t._div4 = true;
		t._offset /= 4;
		return t;
	}

	else
	{
		// Cannot divide Expr by non-4.
		std::abort();
	}
}

Expr operator+(int lhs, const Expr& rhs)
{
	return rhs + lhs;
}


////////

void Expr::resolveForward()
{
	if(_targetType == TARGET_FORWARD)
	{
		Expr fwExpr = _target.forward->getExpr();

		// Pure value copy is permitted here because Expr is really almost-POD type.
		// _target.name is the only member that needs special kind of destructor, where
		// in this case _targetType is NOT TARGET_NAME and the member won't have any
		// meanings.
		if(_div4) *this = fwExpr / 4 + _offset;
		else *this = fwExpr + _offset;
	}
}

TargetType Expr::getTargetType()
{
	resolveForward();
	return _targetType;
}


EUDObject* Expr::getTargetObject()
{
	resolveForward();
	assert(getTargetType() == TARGET_OBJECT);
	return _target.object;
}


std::string Expr::getTargetName()
{
	resolveForward();
	assert(getTargetType() == TARGET_NAME);
	return *_target.name;
}


bool Expr::isTargetDividedBy4()
{
	resolveForward();
	assert(getTargetType() != TARGET_NONE);
	return _div4;
}


uint32_t Expr::getOffset()
{
	resolveForward();
	return _offset;
}

///////

Expr getNameExpr(const std::string& name)
{
	return Expr(name, false, 0);
}
