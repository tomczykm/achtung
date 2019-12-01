#pragma once

#include <SFML/Graphics.hpp>

class TrailThing {
public:
    TrailThing(float x, float y, float angle, float radius, sf::Color);

    const sf::RectangleShape& getShape() const { return shape_; }
    void setColor(sf::Color c) { shape_.setFillColor(c); }
private:
    sf::RectangleShape shape_;
};
