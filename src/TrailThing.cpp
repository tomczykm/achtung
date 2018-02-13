#include "inc/TrailThing.h"

CTrailThing::CTrailThing( float x, float y, int ang, int rad ):
  CThing( x, y ),
  angle( ang ),
  radius( rad )
{

}

SDL_Rect CTrailThing::GetRenderRect() const {
  SDL_Rect rec = { static_cast< int >( xPos ), static_cast< int >( yPos ), radius, radius };
  return rec;
}
