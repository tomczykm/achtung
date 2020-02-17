#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>

class PowerupThing {
public:
    PowerupThing(uint32_t xPos, uint32_t yPos);

    const sf::CircleShape& getShape() const { return shape_; }
private:
    sf::CircleShape shape_;
};