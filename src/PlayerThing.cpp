#include "inc/PlayerThing.h"
#include "inc/Global.h"

#include <cmath>

const float CPlayerThing::TURN_DEG = 240;

CPlayerThing::CPlayerThing( std::string n, SDL_Scancode left, SDL_Scancode right ):
  CThing( 0, 0 ),
  name( n ),
  direction( 0.0 ),
  turnR( false ),
  turnL( false ),
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
  //The pointer returned is a pointer to an internal SDL array.
  //It will be valid for the whole lifetime of the application and should not be freed by the caller.
  const auto keyStates = SDL_GetKeyboardState( nullptr );
  turnL = keyStates[ leftKey ] && !keyStates[ rightKey ];
  turnR = keyStates[ rightKey ];
}

void CPlayerThing::Move( float timeStep ) {
  if( turnL ) direction += timeStep * TURN_DEG;
  else if( turnR ) direction -= timeStep * TURN_DEG;

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

void CPlayerThing::CreateTrail( std::deque <CTrailThing> &trails ) const {
  trails.push_front( CTrailThing( xPos, yPos, direction, radius ) );
}

SDL_Rect CPlayerThing::GetRenderRect() const {
  SDL_Rect r = { static_cast< int >( xPos ), static_cast< int >( yPos ), radius, radius };
  return r;
}
