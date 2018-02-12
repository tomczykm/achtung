#pragma once

#include <string>
#include <SDL.h>

class CPlayerThing {
public:
  CPlayerThing( std::string n );
  ~CPlayerThing();

  void Input();
  void Logic( float timeStep );
  void NewRoundSetup( int xmin, int xmax, int ymin, int ymax );

  SDL_Rect GetRenderRect() const;
  int GetRadius() const { return radius; }

  void DoRoga( int x, int y );
private:
  std::string name;

  double xPos, yPos;
  int direction; //in degrees
  double vel; //pixels per seconds - depends on the size of the playfield

  int radius; //for thickness buffs
  int score;

  bool dead;

  SDL_Scancode leftKey, rightKey;
};
