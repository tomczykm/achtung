#pragma once

#include <SDL.h>
#include <utility>

using Coords = std::pair< double, double >;

class Thing {
public:
  Thing( double xPos, double yPos );
  virtual ~Thing(){}

  Coords GetPos() const;
  virtual SDL_Rect GetRenderRect() const;
protected:
  double xPos_, yPos_;
private:

};
