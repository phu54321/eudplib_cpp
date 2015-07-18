#pragma once

#include "../Core/Expr.hpp"

struct _unk1836759;  // Don't declare.
struct scConst
{
	scConst(const _unk1836759& c) {}
};


// These types can be initialized from string
// AIScript, Location, String, Switch, Unit
#define CREATE_STRCONST_TRIGGER_TYPE(name) \
class name \
{ \
public: \
	name(const Expr& expr); \
	name(const std::string& name); \
	operator Expr(); \
	operator uint16_t(); \
	operator uint8_t(); \
private: \
	Expr _expr; \
};

CREATE_STRCONST_TRIGGER_TYPE(AIScript)
CREATE_STRCONST_TRIGGER_TYPE(Location)
CREATE_STRCONST_TRIGGER_TYPE(String)
CREATE_STRCONST_TRIGGER_TYPE(Switch)
CREATE_STRCONST_TRIGGER_TYPE(Unit)


// These types can be initialized from predefined constants
// AllyStatus, Comparison, Modifier, OrderType, Player, PropState, Resource,
// SwitchAction, SwitchState, Count, ScoreType
#define CREATE_CONST_TRIGGER_TYPE(name) \
class name \
{ \
public: \
	name(const Expr& expr); \
	name(const scConst& scc); \
	name(void* fptr); \
	operator Expr(); \
	operator uint16_t(); \
	operator uint8_t(); \
private: \
	Expr _expr; \
};

CREATE_CONST_TRIGGER_TYPE(AllyStatus)
CREATE_CONST_TRIGGER_TYPE(Comparison)
CREATE_CONST_TRIGGER_TYPE(Modifier)
CREATE_CONST_TRIGGER_TYPE(OrderType)
CREATE_CONST_TRIGGER_TYPE(Player)
CREATE_CONST_TRIGGER_TYPE(PropState)
CREATE_CONST_TRIGGER_TYPE(Resource)
CREATE_CONST_TRIGGER_TYPE(SwitchAction)
CREATE_CONST_TRIGGER_TYPE(SwitchState)
CREATE_CONST_TRIGGER_TYPE(Count)
CREATE_CONST_TRIGGER_TYPE(ScoreType)
