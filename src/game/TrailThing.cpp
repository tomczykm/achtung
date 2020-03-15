#include "game/TrailThing.hpp"

constexpr float TRAIL_HEIGHT = 3;

TrailThing::TrailThing(float x, float y, float ang, float rad, sf::Color c):
    shape_({rad, TRAIL_HEIGHT})
{
    shape_.setOrigin({rad/2, TRAIL_HEIGHT/2});
    shape_.setRotation(ang);
    shape_.setPosition({x, y});
    shape_.setFillColor(c);
}
