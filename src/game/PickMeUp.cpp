#include "game/PickMeUp.hpp"

#include "game/StateGame.hpp"

PickMeUp::PickMeUp(uint32_t xPos, uint32_t yPos, float radius, const sf::Texture& texture, OnPickUp f):
    shape_{radius},
    sprite_{texture},
    onPickUp_{f}
{
    shape_.setOrigin(radius, radius);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);

    const auto texSize = texture.getSize().x;
    const auto scale = radius*2 / static_cast<float>(texSize);

    sprite_.setOrigin(texSize/2, texSize/2);
    sprite_.setPosition(xPos, yPos);
    sprite_.setScale(scale, scale);
}
