#pragma once

#include <string>
#include <SDL.h>

class Texture {
public:
  Texture ( std::string filename = std::string() );
  ~Texture ();

  bool LoadFromFile( std::string filename );
  void Render( int x, int y ) const;
  void Render( int x, int y, int w, int h, double angle = 0.0, SDL_Point *center = nullptr, SDL_Rect *clip = nullptr ) const;
  int GetW() const { return mw_; }
  int GetH() const { return mh_; }
private:
  SDL_Texture *t_;
  int mw_, mh_;
};
