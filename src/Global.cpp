#include "Global.hpp"

#include <cstdlib>

int randomInt(int min, int max) {
    if (min >= max) return max;
    return min + (std::rand() % (max - min + 1));
}
