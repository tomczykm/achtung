#pragma once

#include <string>
#include <SDL.h>

class Texture {
public:
    Texture(std::string filename = std::string());
    ~Texture();

    bool loadFromFile(std::string filename);
    void render(int x, int y) const;
    void render(int x, int y, int w, int h, double angle = 0.0,
        SDL_Point *center = nullptr, SDL_Rect *clip = nullptr) const;
    int getW() const { return mw_; }
    int getH() const { return mh_; }
private:
    SDL_Texture *t_;
    int mw_, mh_;
};
