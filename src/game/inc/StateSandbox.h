#pragma once

#include <deque>

#include "BaseGameState.h"
#include "PlayerThing.h"
#include "TrailThing.h"
#include "Texture.h"

class StateSandbox: public BaseGameState {
public:
    StateSandbox();
    ~StateSandbox();

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
    Texture playerTex_;
    Texture trailTex_;

    PlayerThing player_;
    std::deque <TrailThing> trails_;

  bool move_;

    //for calculating per-pixel movement speed and turn angles
    Uint32 moveTimer_;
};
