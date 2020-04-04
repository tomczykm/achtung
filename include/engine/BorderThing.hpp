#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class BorderThing
{
public:
    BorderThing(float origin, float sideLength);
    using WallShapes = std::array<sf::RectangleShape, 4>;

    const WallShapes& getShapes() const { return shapes_; }

    void setAlpha(std::uint8_t alpha);

private:
    WallShapes shapes_;
};
