#include "Global.hpp"

std::unique_ptr<Logger> logger;

int randomInt(int min, int max) {
    if (min >= max) return max;
    return min + (std::rand() % (max - min + 1));
}
