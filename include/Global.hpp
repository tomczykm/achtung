#pragma once

#include <memory>
#include "Logger.hpp"

extern int randomInt(int min, int max);

extern std::unique_ptr<Logger> logger;
