#include "game/PickMeUp.hpp"

#include "game/StateGame.hpp"

namespace
{
constexpr uint32_t RADIUS = 14;
}

PickMeUp::PickMeUp(uint32_t xPos, uint32_t yPos, const sf::Texture& texture, OnPickUp f):
    shape_{RADIUS},
    sprite_{texture},
    onPickUp_{f}
{
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);

    const auto texSize = texture.getSize().x;
    const auto scale = RADIUS*2 / static_cast<float>(texSize);

    sprite_.setOrigin(texSize/2, texSize/2);
    sprite_.setPosition(xPos, yPos);
    sprite_.setScale(scale, scale);
}
