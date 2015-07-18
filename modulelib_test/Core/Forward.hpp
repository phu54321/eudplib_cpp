#pragma once

class Expr;

class Forward
{
	friend class Expr;

public:
	Forward();
	~Forward();

	void assign(const Expr& expr);

private:
	Expr getExpr() const;
	Expr* _expr;
};
