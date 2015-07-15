#include "Expr.hpp"
#include <cassert>

Expr::Expr(EUDObject* obj, bool div4, uint32_t offset)
	: _usetarget(true), _targetobj(obj), _div4(div4), _offset(offset)
{}

Expr::Expr(const std::string& str, bool div4, uint32_t offset)
	: _usetarget(true), _targetobj(nullptr), _targetname(str), _div4(div4), _offset(offset)
{}

Expr::Expr(uint32_t offset)
	: _offset(offset)
{}

Expr::~Expr() {}

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
		assert(!div4, "Cannot divide object pointer by 4 for more than two times.");
		assert(offset % 4 == 0, "Expr offset is not divisible by 4.");
		
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


TargetType Expr::getTargetType() const
{
	if(!_usetarget) return TARGET_NONE;
	else if(_targetobj) return TARGET_OBJECT;
	else return TARGET_NAME;
}


EUDObject* Expr::getTargetObject() const
{
	assert(getTargetType() == TARGET_OBJECT);
	return const_cast<EUDObject*>(_targetobj);
}


std::string Expr::getTargetName() const
{
	assert(getTargetType() == TARGET_NAME);
	return _targetname;
}


bool Expr::isTargetDividedBy4() const
{
	assert(getTargetType() != TARGET_NONE);
	return _div4;
}


uint32_t Expr::getOffset() const
{
	return _offset;
}
