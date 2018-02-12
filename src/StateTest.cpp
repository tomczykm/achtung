#include "inc/StateTest.h"
#include "inc/Global.h"

CStateTest::CStateTest():
  tex( "dot.png" ),
  tex2( "wall.png" ),
  radius( 12 )
{

}

CStateTest::~CStateTest() {

}

void CStateTest::Input() {

}

void CStateTest::Logic() {

}

void CStateTest::Render() {
  int wt = 4; //wall thickness
  int x = 0.05 * settings->GetResH();
  int y = x;
  int w = 0.9 * settings->GetResH();
  int h = w;

  //drawing outer walls
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF );
  SDL_Rect rect;
  //top
  rect = { x, y, w, wt };
  SDL_RenderFillRect( renderer, &rect );
  //bottom
  rect = { x, y+h-wt, w, wt };
  SDL_RenderFillRect( renderer, &rect );
  //left
  rect = { x, y, wt, h };
  SDL_RenderFillRect( renderer, &rect );
  //right
  rect = { x+w-wt, y, wt, h };
  SDL_RenderFillRect( renderer, &rect );
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

  tex.Render( 100, 100, radius, radius );
  tex2.Render( 120, 100, radius*1.5, radius, 60 );
  tex2.Render( 130, 100, radius*1.5, radius, 90 );
  tex2.Render( 140, 100, radius*1.5, radius, 180 );
}
