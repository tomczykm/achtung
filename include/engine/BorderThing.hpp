#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class BorderThing
{
public:
    BorderThing(float origin, float sideLength);
    using WallShapes = std::array<sf::RectangleShape, 4>;
    using Drawables = std::array<const sf::Drawable*, 8>;

    const WallShapes& getShapes() const { return shapes_; }
    const Drawables& getDrawables() const { return drawables_; }

    void setAlpha(std::uint8_t alpha);

private:
    WallShapes shapes_;
    WallShapes blackBars_;  // for covering up pieces of trails sticking outside after warping

    Drawables drawables_;
};
