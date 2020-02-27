#include "game/PickMeUp.hpp"

namespace
{
constexpr uint32_t RADIUS = 14;
}

PickMeUp::PickMeUp(uint32_t xPos, uint32_t yPos):
    shape_{RADIUS}
{
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);
}
