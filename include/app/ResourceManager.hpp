#pragma once

#include <sstream>
#include <string>
#include <string_view>

namespace ResourceManager {

std::string getActualFileName(std::string_view resourceName);
std::stringstream openResource(std::string_view resourceName);

}
