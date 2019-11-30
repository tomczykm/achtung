#pragma once

#include <SFML/Graphics.hpp>

class TrailThing {
public:
    TrailThing(float x, float y, float angle, float radius, sf::Color);

    const sf::RectangleShape& getShape() const { return shape_; }
private:
    sf::RectangleShape shape_;
};
