#include "TrailThing.h"

TrailThing::TrailThing( float x, float y, int ang, int rad ):
  Thing( x, y ),
  angle_( ang ),
  radius_( rad )
{

}

SDL_Rect TrailThing::GetRenderRect() const {
  SDL_Rect rec = { static_cast< int >( xPos_ ), static_cast< int >( yPos_ ), radius_, radius_ };
  return rec;
}
