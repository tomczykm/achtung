#include "StateSandbox.h"
#include "Global.h"

StateSandbox::StateSandbox():
  playerTex_( "dot.png" ),
  trailTex_( "wall.png" ),
  player_( "player", SDL_SCANCODE_Q, SDL_SCANCODE_W ),
  move_( false ),
  moveTimer_( SDL_GetTicks() )
{
  player_.NewRoundSetup( 100, 600, 100, 600 );
}

StateSandbox::~StateSandbox() {

}

void StateSandbox::Input() {
  player_.Input();
}

void StateSandbox::PolledInput() {
  if( events.type == SDL_KEYDOWN ) {
		switch( events.key.keysym.sym ) {
		case SDLK_SPACE:
      move_ = true;
    }
  }
  if( events.type == SDL_KEYUP ) {
    switch( events.key.keysym.sym ) {
    case SDLK_SPACE:
      move_ = false;
    }
  }
}


void StateSandbox::Logic() {
  if( move_ ) {
    player_.Move( ( SDL_GetTicks() - moveTimer_ ) / 1000.f );
  }

  for( const auto& t: trails_ ) {
    if( player_.CheckCollision( t ) ) {
      logger->Out( "Collsion!" );
    }
  }
  moveTimer_ = SDL_GetTicks();
}

void StateSandbox::Render() {
  for( auto &t: trails_ ) {
    auto rec = t.GetRenderRect();
    trailTex_.Render( rec.x, rec.y, rec.w, rec.h, t.GetAngle() );
  }

  auto rec = player_.GetRenderRect();
  playerTex_.Render( rec.x, rec.y, rec.w, rec.h );
}
