#pragma once

#include <sstream>
#include <string_view>

namespace ResourceManager {

std::stringstream openResource(std::string_view resourceName);

}