#include <SDL_image.h>

#include "Texture.hpp"
#include "Log.hpp"

Texture::Texture(SDL_Renderer *renderer, std::string filename = std::string()):
    renderer_(renderer),
    t_(nullptr),
    mw_(0),
    mh_(0)
{
    if(!filename.empty()) {
        loadFromFile(filename);
    }
}

Texture::~Texture() {
    if(t_ != nullptr) {
        SDL_DestroyTexture(t_);
        t_ = nullptr;
        mw_ = 0;
        mh_ = 0;
    }
}

bool Texture::loadFromFile(std::string filename) {
    this->~Texture();

    SDL_Texture *newTexture = nullptr;
    SDL_Surface *loadedSurface = IMG_Load(filename.c_str());

    if (!loadedSurface) {
        log_ << error << "Error loading image \"" << filename << "\". IMG_Error: " << IMG_GetError();
        return false;
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(renderer_, loadedSurface);

    if (!newTexture) {
        log_ << error << "Error creating texture from \"" << filename << "\"! SDL_Error: " <<  SDL_GetError();
        return false;
    }

    mw_ = loadedSurface->w;
    mh_ = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);

    t_ = newTexture;
    return t_ != nullptr;
}

void Texture::render(int x, int y) const {
    SDL_Rect rq = {x, y, mw_, mh_};
    SDL_RenderCopy(renderer_, t_, nullptr, &rq);
}

void Texture::render(int x, int y, int w, int h, double angle, SDL_Point *center, SDL_Rect *clip) const {
    SDL_Rect rq = {x, y, w, h};
    if (clip != nullptr) {
        rq.w = clip->w;
        rq.h = clip->h;
    }
    SDL_RenderCopyEx(renderer_, t_, clip, &rq, angle, center, SDL_FLIP_NONE);
}
