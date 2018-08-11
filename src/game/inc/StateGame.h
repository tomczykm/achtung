#pragma once

#include "BaseGameState.h"
#include "Texture.h"
#include "Global.h"
#include "PlayerThing.h"

#include <vector>
#include <deque>

enum class GameplayStatus {
    null = 0,
    roundBegin, //at the start of each round; move to RUNNING
    running, //when everybody is furiously trying to kill each other; move to PAUSE, ROUND_END or GAME_END
    pause, //when a player needs a break; move to RUNNING or quit the game in progress
    roundEnd, //when there's only one standing; move to ROUND_BEGIN
    gameEnd //when a player has reached the score goal and is declared a winner; move to another gamestate (TODO)
};

class StateGame: public BaseGameState {
public:
    StateGame();
    ~StateGame();

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
    Texture playerTex_;
    Texture trailTex_;

    //dimensions for border
    //cannot be static const as they use SettingsHandler::GetResH() to initialize
    const int wt_, bx_, bw_;

    std::vector <PlayerThing> players_;
    std::deque <TrailThing> trails_;

    //for calculating per-pixel movement speed and turn angles
    Uint32 moveTimer_;

    GameplayStatus status_;
};
