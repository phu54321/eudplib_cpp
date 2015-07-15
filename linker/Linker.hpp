#pragma once

#include "Payload.hpp"
#include "Module.hpp"

PayloadPtr linkModules(const std::vector<ModulePtr>& modules);
