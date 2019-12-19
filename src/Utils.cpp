#include "Utils.hpp"

#include <ctime>
#include <cctype>

namespace xor_rand {
namespace {
    // PRNG state
    Type a, b, c, d;
}

void seed(Type seedA, Type seedB, Type seedC, Type seedD) {
    a = seedA;
    b = seedB;
    c = seedC;
    d = seedD;
    discard(5000);
}

// Marsaglia's xorshift
Type next(Type min, Type max) {
    if (min >= max) return max;

    auto t = d;
	auto s = a;

	d = c;
	c = b;
	b = s;

	t ^= t << 11;
	t ^= t >> 8;
	a = t ^ s ^ (s >> 19);

    return min + (a % (max - min + 1));
}

void discard(std::size_t n) {
    for(std::size_t i = 0u; i < n; ++i) next(0, 1);
}

}  // namespace random

std::string_view stripLeadingDigits(std::string_view s) {
    auto it = s.begin();
    while (std::isdigit(*it)) it++;
    return std::string_view{it, static_cast<size_t>(s.end()-it)};
}