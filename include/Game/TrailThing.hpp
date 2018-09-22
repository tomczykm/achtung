#pragma once

#include "Game/Thing.hpp"

class TrailThing : public Thing {
public:
    TrailThing(double x, double y, int ang, int rad);
    SDL_Rect getRenderRect() const override;

    double getAngle() const { return angle_; }
private:
    double angle_;
    int radius_;
};
