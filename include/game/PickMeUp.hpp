#pragma once

#include <SFML/Graphics.hpp>

class PickMeUp {
public:
    PickMeUp(uint32_t xPos, uint32_t yPos);

    const sf::CircleShape& getShape() const { return shape_; }
private:
    sf::CircleShape shape_;
};