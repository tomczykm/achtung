#pragma once

#include <string_view>
#include <SFML/Graphics.hpp>

namespace xor_rand {

using Type = std::uint32_t;

void seed(Type, Type, Type, Type);
Type next(Type min, Type max);
void discard(std::size_t n);

}

std::string_view stripLeadingDigits(std::string_view s);

std::string_view keycodeToStr(sf::Keyboard::Key);

float distance(sf::Vector2f p1, sf::Vector2f p2);
