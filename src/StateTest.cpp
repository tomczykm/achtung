#include "inc/StateTest.h"

CStateTest::CStateTest():
  playerTex( "dot.png" ),
  trailTex( "wall.png" ),
  wt( 4 ),
  bx( 0.05 * settings->GetResH() ),
  bw( 0.9 * settings->GetResH() ),
  moveTimer( SDL_GetTicks() ),
  status( GameplayStatus::roundBegin )
{
  players.push_back( CPlayerThing( "player", SDL_SCANCODE_Q, SDL_SCANCODE_W ) );
  for( auto it = players.begin() ; it != players.end() ; it++ ) {
    it->NewRoundSetup( 100, 600, 100, 600 );
  }
  logger->Out( "Szerokość pola to " + ToString( bw ) + "px" );
}

CStateTest::~CStateTest() {

}

void CStateTest::Input() {
  for( auto it = players.begin() ; it != players.end() ; it++ ) {
    it->Input();
  }
}

void CStateTest::PolledInput() {
  if( events.type == SDL_KEYDOWN ) {
		switch( events.key.keysym.sym ) {
		case SDLK_SPACE:
      SDL_Rect rec = trails.cbegin()->GetRenderRect();
      logger->Out( "rec x: " + ToString( rec.w ) );
    }
  }
}

void CStateTest::Logic() {
  for( auto it = players.begin() ; it != players.end() ; it++ ) {
    //move players
    it->Move( ( SDL_GetTicks() - moveTimer ) / 1000.f );

    //create trails
    it->CreateTrail( &trails );

    //handle collisions
  }
  moveTimer = SDL_GetTicks();
}

void CStateTest::Render() {
  //draw trails
  for( auto it = trails.cbegin() ; it != trails.cend() ; it++ ) {
    SDL_Rect rec = it->GetRenderRect();
    trailTex.Render( rec.x, rec.y, rec.w, rec.h, static_cast< double >( it->GetAngle() ) );
  }

  //draw players
  for( auto it = players.cbegin() ; it != players.cend() ; it++ ) {
    SDL_Rect rec = it->GetRenderRect();
    playerTex.Render( rec.x, rec.y, rec.w, rec.h );
  }

  //drawing outer walls
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
  SDL_Rect rect;
  //top
  rect = { bx, bx, bw, wt };
  SDL_RenderFillRect( renderer, &rect );
  //bottom
  rect = { bx, bx+bw-wt, bw, wt };
  SDL_RenderFillRect( renderer, &rect );
  //left
  rect = { bx, bx, wt, bw };
  SDL_RenderFillRect( renderer, &rect );
  //right
  rect = { bx+bw-wt, bx, wt, bw };
  SDL_RenderFillRect( renderer, &rect );
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
}
