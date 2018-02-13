#pragma once

#include "Thing.h"

class CTrailThing: public CThing {
public:
  CTrailThing( float x, float y, int ang, int rad );
  SDL_Rect GetRenderRect() const;

  int GetAngle() const { return angle; }
private:
  int angle;
  int radius;
};
