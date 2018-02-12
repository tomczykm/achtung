#include "inc/PlayerThing.h"
#include "inc/Global.h"

#include <cmath>

CPlayerThing::CPlayerThing( std::string n ):
  name( n ),
  xPos( 0 ),
  yPos( 0 ),
  direction( 0.0 ),
  vel( 50 ),
  radius( 10 ),
  score( 0 ),
  dead( false )
{}

CPlayerThing::~CPlayerThing() {

}

void CPlayerThing::Input() {

}

void CPlayerThing::Logic( float timeStep ) {
  xPos += timeStep * vel * sin( (M_PI/180)*direction );
	yPos += timeStep * vel * cos( (M_PI/180)*direction );
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
