#include "engine/TrailThing.hpp"

TrailThing::TrailThing(float x, float y, float ang, float rad, sf::Color c):
    shape_({rad, height})
{
    shape_.setOrigin({rad/2, height/2});
    shape_.setRotation(ang);
    shape_.setPosition({x, y});
    shape_.setFillColor(c);
}
