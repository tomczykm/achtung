#include "game/PickMeUp.hpp"

#include "game/StateGame.hpp"

namespace
{
constexpr uint32_t RADIUS = 14;
}

PickMeUp::PickMeUp(uint32_t xPos, uint32_t yPos, OnPickUp f):
    shape_{RADIUS},
    onPickUp_{f}
{
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);
}
