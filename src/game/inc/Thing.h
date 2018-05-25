#pragma once

#include <SDL.h>

class Thing {
public:
  Thing( double xPos, double yPos );
  virtual ~Thing(){}

  virtual SDL_Rect GetRenderRect() const;
protected:
  double xPos_, yPos_;
private:

};
