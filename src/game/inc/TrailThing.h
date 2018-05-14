#pragma once

#include "Thing.h"

class TrailThing: public Thing {
public:
  TrailThing( float x, float y, int ang, int rad );
  SDL_Rect GetRenderRect() const;

  double GetAngle() const { return angle_; }
private:
  double angle_;
  int radius_;
};
