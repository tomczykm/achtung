#include "PlayerThing.h"

#include <cmath>

#include "Global.h"

using namespace std::chrono;

const float PlayerThing::TURN_DEG( 160 );
const milliseconds PlayerThing::GAP_TIME( 190 ); //in ms

PlayerThing::PlayerThing( std::string n, SDL_Scancode left, SDL_Scancode right ):
  Thing( 0, 0 ),
  name_( n ),
  direction_( 0.0 ),
  turnR_( false ),
  turnL_( false ),
  vel_( 100 ),
  radius_( 7 ),
  score_( 0 ),
  dead_( false ),
  leftKey_( left ),
  rightKey_( right ),
  gap_( false ),
  switch_time_()
{}

PlayerThing::~PlayerThing() {

}

void PlayerThing::Input() {
  //The pointer returned is a pointer to an internal SDL array.
  //It will be valid for the whole lifetime of the application and should not be freed by the caller.
  const auto keyStates = SDL_GetKeyboardState( nullptr );
  turnL_ = keyStates[ leftKey_ ] && !keyStates[ rightKey_ ];
  turnR_ = keyStates[ rightKey_ ];
}

void PlayerThing::Move( float timeStep ) {
  if( turnL_ ) direction_ -= timeStep * TURN_DEG;
  else if( turnR_ ) direction_ += timeStep * TURN_DEG;
  if( direction_ < 0 ) direction_ += 360;
  else if( direction_ >= 360 ) direction_ -= 360;

  if( steady_clock::now() >= switch_time_ )
    GapSwitch();

  xPos_ += timeStep * vel_ * sin( -(M_PI/180)*direction_ );
	yPos_ += timeStep * vel_ * cos( -(M_PI/180)*direction_ );
}

//places a player randomly on the playfield and moves them a couple steps
//so that the player can tell the direction they're going to move
void PlayerThing::NewRoundSetup( int xmin, int xmax, int ymin, int ymax ) {
  dead_ = false;
  xPos_ = RandomInt( xmin, xmax );
  yPos_ = RandomInt( ymin, ymax );
  direction_ = std::rand()%360;
  
  //todo: move the player

  gap_ = true;
  GapSwitch();
}

void PlayerThing::CreateTrail( std::deque <TrailThing> &trails ) const {
  if( !gap_ )
    trails.emplace_front( xPos_, yPos_, direction_, radius_ );
}

SDL_Rect PlayerThing::GetRenderRect() const {
  SDL_Rect r = { static_cast< int >( xPos_ ), static_cast< int >( yPos_ ), radius_, radius_ };
  return r;
}

void PlayerThing::GapSwitch() {
  gap_ = !gap_;
  if( gap_ ) {
    switch_time_ = steady_clock::now() + GAP_TIME;
  } else {
    switch_time_ = steady_clock::now() + milliseconds( RandomInt( 3500, 6500 ) );
  }
}
