#pragma once

#include "Payload.hpp"
#include "../Module/Module.hpp"

PayloadPtr linkModules(const std::vector<ModulePtr>& modules);
