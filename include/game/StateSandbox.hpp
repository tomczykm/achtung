#pragma once

#include <deque>

#include "Application.hpp"
#include "IGameState.hpp"
#include "game/PlayerThing.hpp"
#include "game/TrailThing.hpp"
#include "Texture.hpp"

class StateSandbox: public IGameState {
public:
    StateSandbox(Application&);

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
    Application& app_;

    Texture playerTex_{app_.renderer(), "dot.png"};
    Texture trailTex_{app_.renderer(), "wall.png"};

    PlayerThing player_{"player", SDL_SCANCODE_Q, SDL_SCANCODE_W};
    std::deque <TrailThing> trails_;

    bool move_ = false;

    // for calculating per-pixel movement speed and turn angles
    std::uint32_t moveTimer_ = SDL_GetTicks();
};
