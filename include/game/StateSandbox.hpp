#pragma once

#include <deque>

#include "Application.hpp"
#include "IGameState.hpp"
#include "game/PlayerThing.hpp"
#include "game/TrailThing.hpp"

class StateSandbox: public IGameState {
public:
    StateSandbox(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override;
    void render() override;
private:
    Application::Interface app_;

    PlayerThing player_{"player"/*, SDL_SCANCODE_Q, SDL_SCANCODE_W*/};
    std::deque<TrailThing> trails_;

    bool move_ = false;

    // for calculating per-pixel movement speed and turn angles
    std::uint32_t moveTimer_ = /*SDL_GetTicks()*/0;
};
