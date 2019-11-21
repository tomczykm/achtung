#include "game/TrailThing.hpp"

TrailThing::TrailThing(double x, double y, int ang, int rad):
    xPos_(x),
    yPos_(y),
    angle_(ang),
    radius_(rad)
{}

SDL_Rect TrailThing::getRenderRect() const {
    return {static_cast<int>(xPos_-radius_/2), static_cast<int>(yPos_-radius_/2), radius_, radius_};
}
