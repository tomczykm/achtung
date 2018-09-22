#include "Game/Thing.hpp"

Thing::Thing(double x, double y):
    xPos_(x),
    yPos_(y)
{

}

Coords Thing::getPos() const {
    return {xPos_, yPos_};
}

SDL_Rect Thing::getRenderRect() const {
    //when w and h are 0, the texture rendered is not to be scaled
    SDL_Rect rec = {static_cast<int>(xPos_ ), static_cast<int>( yPos_), 0, 0};
    return rec;
}