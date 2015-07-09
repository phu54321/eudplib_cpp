#pragma once
#include "../Common/Payload.hpp"
#include "../Common/Module.hpp"

PayloadPtr linkModules(const std::vector<ModulePtr>& modules);
