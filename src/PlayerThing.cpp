#include "inc/PlayerThing.h"
#include "inc/Global.h"

#include <cmath>

CPlayerThing::CPlayerThing( std::string n, SDL_Scancode left, SDL_Scancode right ):
  name( n ),
  xPos( 0 ),
  yPos( 0 ),
  direction( 0.0 ),
  vel( 150 ),
  radius( 10 ),
  score( 0 ),
  dead( false ),
  leftKey( left ),
  rightKey( right )
{}

CPlayerThing::~CPlayerThing() {

}

void CPlayerThing::Input() {
  const Uint8 *keyStates = SDL_GetKeyboardState( nullptr );
	if( keyStates[ leftKey ] ) {
		if( !keyStates[ rightKey ] )
		  direction += 3;
	} else if ( keyStates[ rightKey ] ) {
		direction -= 3;
	}
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
