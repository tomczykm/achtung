#include "TrailThing.h"

TrailThing::TrailThing(double x, double y, int ang, int rad):
    Thing(x, y),
    angle_(ang),
    radius_(rad)
{

}

SDL_Rect TrailThing::getRenderRect() const {
    SDL_Rect rec = {static_cast<int>(xPos_-radius_/2 ), static_cast<int>(yPos_-radius_/2),
        radius_, radius_};
    return rec;
}
