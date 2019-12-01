#include "Utils.hpp"

#include <cstdlib>
#include <cctype>

int randomInt(int min, int max) {
    if (min >= max) return max;
    return min + (std::rand() % (max - min + 1));
}

std::string_view stripLeadingNumbers(std::string_view s) {
    auto it = s.begin();
    while (std::isdigit(*it)) it++;
    return std::string_view{it, static_cast<size_t>(s.end()-it)};
}