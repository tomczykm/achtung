#pragma once

#include "Application.hpp"
#include "IGameState.hpp"
#include "Texture.hpp"
#include "game/PlayerThing.hpp"

#include <vector>
#include <deque>

enum class GameplayStatus {
    null = 0,
    roundBegin, // at the start of each round; move to RUNNING
    running, // when everybody is furiously trying to kill each other; move to PAUSE, ROUND_END or GAME_END
    pause, // when a player needs a break; move to RUNNING or quit the game in progress
    roundEnd, // when there's only one standing; move to ROUND_BEGIN
    gameEnd // when a player has reached the score goal and is declared a winner; move to another gamestate (TODO)
};

class StateGame: public IGameState {
public:
    StateGame(Application&);

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;

private:
    Application& app_;

    Texture playerTex_{app_.renderer(), "dot.png"};
    Texture trailTex_{app_.renderer(), "wall.png"};

    // dimensions for border
    static constexpr int wt_ = 4;
    const int bx_ = 0.05 * app_.settings().getResH();
    const int bw_ = 0.9 * app_.settings().getResH();

    std::vector<PlayerThing> players_;
    std::vector<PlayerThing>::iterator lastAlive_;

    std::deque<TrailThing> trails_;

    // for calculating per-pixel movement speed and turn angles
    std::uint32_t moveTimer_ = SDL_GetTicks();

    // GameplayStatus status_ = GameplayStatus::roundBegin;
};
