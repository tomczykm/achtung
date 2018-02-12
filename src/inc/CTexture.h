#pragma once

#include <string>
#include <SDL.h>

class CTexture {
public:
  CTexture    ( std::string filename = std::string() );
  ~CTexture   ();

  bool          LoadFromFile( std::string filename );
  void          Render( int x, int y ) const;
  void          Render( int x, int y, int w, int h, double angle = 0.0, SDL_Point *center = nullptr, SDL_Rect *clip = nullptr ) const;
  int           GetW() const { return mw; }
  int           GetH() const { return mh; }
private:
  SDL_Texture   *t;
  int           mw, mh;
};
