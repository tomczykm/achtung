#include "game/PowerupThing.hpp"

namespace
{
constexpr uint32_t RADIUS = 14;
}

PowerupThing::PowerupThing(uint32_t xPos, uint32_t yPos):
    shape_{RADIUS}
{
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);
}