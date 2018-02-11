#include "inc/CTexture.h"
#include "inc/Global.h"

CTexture::CTexture( std::string filename ):
  t( nullptr ),
  w( 0 ),
  h( 0 )
{
  if( !filename.empty() ) {
    LoadFromFile( filename );
  }
}

CTexture::~CTexture() {
  if( t != nullptr ) {
    SDL_DestroyTexture( t );
    t = nullptr;
    w = 0;
    h = 0;
  }
}

bool CTexture::LoadFromFile( std::string filename ) {
  this->~CTexture();

  SDL_Texture *newTexture = nullptr;
  SDL_Surface *loadedSurface = IMG_Load( filename.c_str() );

  if( !loadedSurface ) {
    logger->Error( "Error loading image \"" + filename + "\"! IMG_Error: " + std::string( IMG_GetError() ) );
    return false;
  }

  SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
  newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );

  if( !newTexture ) {
    logger->Error( "Error creating texture from \"" + filename + "\"! SDL_Error: " + std::string( SDL_GetError() ) );
    return false;
  }

  w = loadedSurface->w;
  h = loadedSurface->h;
  SDL_FreeSurface( loadedSurface );

  t = newTexture;
  return t != nullptr;
}

void CTexture::Render( int x, int y ) const {
  SDL_Rect rq = { x, y, w, h };
  SDL_RenderCopy( renderer, t, nullptr, &rq );
}
