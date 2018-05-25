#pragma once
#include <SDL.h>

namespace col {

struct Circle {
  double x, y;
  int r;
};

bool CheckCollision( Circle c, SDL_Rect r, double r_angle );
bool CheckCollision( Circle c1, Circle c2 );

};
