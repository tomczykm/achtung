#pragma once

#include <string>
#include <SDL.h>
#include <deque>

#include "Thing.h"
#include "TrailThing.h"

class CPlayerThing: public CThing {
public:
  CPlayerThing( std::string n, SDL_Scancode left, SDL_Scancode right );
  ~CPlayerThing();

  void Input();
  void Move( float timeStep );

  void NewRoundSetup( int xmin, int xmax, int ymin, int ymax );
  void CreateTrail( std::deque <CTrailThing> *trails ) const;

  SDL_Rect GetRenderRect() const;
  int GetRadius() const { return radius; }
private:
  std::string name;

  double direction; //in degrees
  bool turnR, turnL;

  static const float TURN_DEG;
  int vel; //pixels per seconds - depends on the size of the playfield

  int radius; //for thickness buffs
  int score;

  bool dead;

  SDL_Scancode leftKey, rightKey;
};
