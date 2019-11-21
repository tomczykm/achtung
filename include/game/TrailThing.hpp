#pragma once

#include <SDL.h>

class TrailThing {
public:
    TrailThing(double x, double y, int ang, int rad);
    SDL_Rect getRenderRect() const;

    double getAngle() const { return angle_; }
private:
    double xPos_, yPos_;
    double angle_;
    int radius_;
};
