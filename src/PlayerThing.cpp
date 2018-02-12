#include "inc/PlayerThing.h"
#include "inc/Global.h"

#include <cmath>

CPlayerThing::CPlayerThing( std::string n ):
  name( n ),
  xPos( 0 ),
  yPos( 0 ),
  direction( 0 ),
  vel( 691 ),
  radius( 10 ),
  score( 0 ),
  dead( false )
{}

CPlayerThing::~CPlayerThing() {

}

void CPlayerThing::Input() {

}

void CPlayerThing::Logic( float timeStep ) {
  xPos += timeStep * vel * sin( (180/M_PI)*direction );
	yPos += timeStep * vel * cos( (180/M_PI)*direction );
}

//places a player randomly on the playfield and moves them four steps
void CPlayerThing::NewRoundSetup( int xmin, int xmax, int ymin, int ymax ) {
  dead = false;
  xPos = RandomInt( xmin, xmax );
  yPos = RandomInt( ymin, ymax );
  direction = std::rand();
  logger->Out( "Direction: " + ToString( direction ) );
}

SDL_Rect CPlayerThing::GetRenderRect() const {
  SDL_Rect r = { static_cast< int >( xPos ), static_cast< int >( yPos ), radius, radius };
  return r;
}

void CPlayerThing::DoRoga( int x, int y ) {
  xPos = x;
  yPos = y;
  direction = 0;
}
