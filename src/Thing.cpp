#include "inc/Thing.h"

CThing::CThing( float x, float y ):
  xPos( x ),
  yPos( y )
{

}

SDL_Rect CThing::GetRenderRect() const {
  //when w and h are 0, the texture rendered is not to be scaled
  SDL_Rect rec = { static_cast< int >( xPos ), static_cast< int >( yPos ), 0, 0 };
  return rec;
}
