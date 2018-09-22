#pragma once

#include <SDL.h>

namespace col {

struct Circle {
    double x, y;
    int r;
};

bool checkCollision(Circle c, SDL_Rect r, double r_angle);
bool checkCollision(Circle c1, Circle c2);

};
