#pragma once

#include <deque>

#include "Application.hpp"
#include "IGameState.hpp"
#include "Game/PlayerThing.hpp"
#include "Game/TrailThing.hpp"
#include "Texture.hpp"

class StateSandbox: public IGameState {
public:
    StateSandbox(Application& app);
    ~StateSandbox();

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
    Application& app_;

    Texture playerTex_;
    Texture trailTex_;

    PlayerThing player_;
    std::deque <TrailThing> trails_;

    bool move_;

    // for calculating per-pixel movement speed and turn angles
    Uint32 moveTimer_;
};
