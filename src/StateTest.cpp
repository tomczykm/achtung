#include "inc/StateTest.h"

CStateTest::CStateTest():
  playerTex( "dot.png" ),
  wallTex( "wall.png" ),
  wt( 4 ),
  bx( 0.05 * settings->GetResH() ),
  bw( 0.9 * settings->GetResH() ),
  timer( SDL_GetTicks() )
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

}

void CStateTest::Logic() {
  for( auto it = players.begin() ; it != players.end() ; it++ ) {
    it->Logic( ( SDL_GetTicks() - timer ) / 1000.f );
  }
  timer = SDL_GetTicks();
}

void CStateTest::Render() {
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

  //draw players
  for( auto it = players.begin() ; it != players.end() ; it++ ) {
    SDL_Rect rec = it->GetRenderRect();
    playerTex.Render( rec.x, rec.y, rec.w, rec.h );
  }
}
