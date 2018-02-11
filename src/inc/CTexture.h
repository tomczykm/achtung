#pragma once

#include <string>
#include <SDL.h>

class CTexture {
public:
  CTexture    ( std::string filename = std::string() );
  ~CTexture   ();

  bool          LoadFromFile( std::string filename );
  void          Render( int x, int y) const;
  int           GetW() const { return w; }
  int           GetH() const { return h; }
private:
  //void          Free();

  SDL_Texture   *t;
  int           w, h;
};
