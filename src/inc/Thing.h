#pragma once

#include <SDL.h>

class CThing {
public:
  CThing( float xPos, float yPos );
  virtual ~CThing(){}

  virtual SDL_Rect GetRenderRect() const;
protected:
  float xPos, yPos;
private:

};
