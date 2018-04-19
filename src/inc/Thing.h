#pragma once

#include <SDL.h>

class Thing {
public:
  Thing( float xPos, float yPos );
  virtual ~Thing(){}

  virtual SDL_Rect GetRenderRect() const;
protected:
  double xPos_, yPos_;
private:

};
