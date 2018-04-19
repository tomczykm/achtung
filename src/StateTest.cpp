#include "inc/StateTest.h"

StateTest::StateTest():
  playerTex_( "dot.png" ),
  trailTex_( "wall.png" ),
  wt_( 4 ),
  bx_( 0.05 * settings->GetResH() ),
  bw_( 0.9 * settings->GetResH() ),
  moveTimer_( SDL_GetTicks() ),
  status_( GameplayStatus::roundBegin )
{
  players_.emplace_back( "player", SDL_SCANCODE_Q, SDL_SCANCODE_W );
  for( auto &p: players_ ) {
    p.NewRoundSetup( 100, 600, 100, 600 );
  }
  logger->Out( "Szerokość pola to " + ToString( bw_ ) + "px" );
}

StateTest::~StateTest() {

}

void StateTest::Input() {
  for( auto &p: players_ ) {
    p.Input();
  }
}

void StateTest::PolledInput() {
  if( events.type == SDL_KEYDOWN ) {
		switch( events.key.keysym.sym ) {
		case SDLK_SPACE:
      //space for shitty debugging
      ;
    }
  }
}

void StateTest::Logic() {
  for( auto &p: players_ ) {
    //move players
    p.Move( ( SDL_GetTicks() - moveTimer_ ) / 1000.f );

    //create trails
    p.CreateTrail( trails_ );

    //handle collisions
  }
  moveTimer_ = SDL_GetTicks();
}

void StateTest::Render() {
  //draw trails
  for( auto &t: trails_ ) {
    SDL_Rect rec = t.GetRenderRect();
    trailTex_.Render( rec.x, rec.y, rec.w, rec.h, t.GetAngle() );
  }

  //draw players
  for( auto &p: players_ ) {
    SDL_Rect rec = p.GetRenderRect();
    playerTex_.Render( rec.x, rec.y, rec.w, rec.h );
  }

  //drawing outer walls
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
  SDL_Rect rect;
  //top
  rect = { bx_, bx_, bw_, wt_ };
  SDL_RenderFillRect( renderer, &rect );
  //bottom
  rect = { bx_, bx_+bw_-wt_, bw_, wt_ };
  SDL_RenderFillRect( renderer, &rect );
  //left
  rect = { bx_, bx_, wt_, bw_ };
  SDL_RenderFillRect( renderer, &rect );
  //right
  rect = { bx_+bw_-wt_, bx_, wt_, bw_ };
  SDL_RenderFillRect( renderer, &rect );
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
}
