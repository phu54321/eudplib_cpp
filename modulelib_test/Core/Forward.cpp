#include "Forward.hpp"
#include "Expr.hpp"
#include <cassert>

Forward::Forward() : _expr(nullptr) {}
Forward::~Forward()
{
	delete _expr;
}

void Forward::assign(const Expr& expr)
{
	assert(_expr == nullptr);
	_expr = new Expr(expr);
}

Expr Forward::getExpr() const
{
	assert(_expr != nullptr);
	return *_expr;
}
