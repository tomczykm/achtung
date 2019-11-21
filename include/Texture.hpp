#pragma once

#include <string_view>
#include <SDL.h>

class Texture {
public:
    Texture(SDL_Renderer* renderer, std::string_view filename);
    ~Texture();

    bool loadFromFile(std::string_view filename);
    void render(int x, int y) const;
    void render(int x, int y, int w, int h, double angle = 0.0,
        SDL_Point *center = nullptr, SDL_Rect *clip = nullptr) const;
    int getW() const { return mw_; }
    int getH() const { return mh_; }
private:
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *t_ = nullptr;
    int mw_ = 0, mh_ = 0;
};
