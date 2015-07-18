#pragma once

#include "PayloadWriter.hpp"
#include <memory>

class EUDObject;
struct Module;
typedef std::shared_ptr<Module> ModulePtr;

struct ModuleWriterState;

class ModuleWriter : public PayloadWriter
{
public:
	ModuleWriter();
	virtual ~ModuleWriter();

	virtual void writeByte(uint8_t ch);
	virtual void writeWord(uint16_t i);
	virtual void writeDword(uint32_t i);
	virtual void writeDword(const Expr& expr);
	virtual void writeBytes(const void* data, size_t len);
	virtual void writeBytes(const bytes& b);

	ModulePtr createModule(const std::string& moduleIdentifier, EUDObject* root);

private:
	ModuleWriterState* _state;
};
