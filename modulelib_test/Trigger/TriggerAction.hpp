#pragma once

#include "../Core/Expr.hpp"
#include "TriggerConstants.hpp"

class Trigger;
class PayloadWriter;

class Action : public Expr
{
public:
	Action(Expr a, Expr b, Expr c, Expr d, Expr e, Expr f, uint16_t g, uint8_t h, uint8_t i, uint8_t j);
	~Action() {}

	void Disable();
	void SetParentTrigger(Trigger* trigger, int actindex);
	Expr Evaluate();
	void writePayload(PayloadWriter* pw);

private:
	Expr _a, _b, _c, _d, _e, _f;
	uint16_t _g;
	uint8_t _h, _i, _j;

	Trigger* _parentTrigger;
	int _actindex;
};
