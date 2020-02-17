#pragma once

#include "Application.hpp"
#include "IGameState.hpp"
#include "game/Player.hpp"
#include "game/BorderThing.hpp"
#include "game/PowerupThing.hpp"

#include <vector>
#include <deque>

class StateGame: public IGameState {
public:
    StateGame(const Application::Interface&, const std::vector<PlayerInfo>&);
    ~StateGame();

    void input(const sf::Event&) override;
    void logic() override;
    void render() override;

private:
    class RoundState;

    void initializePlayers(const std::vector<PlayerInfo>&);
    void loadGui();
    void sortScoreList();

    bool checkCollisions(PlayerThing& player);  // returns true if they died
    void awardPoints();
    bool victoryGoalAchieved();

    void resetPowerupSpawnTimer();

    template <typename State>
    void changeState() { state_ = std::make_unique<State>(*this); }

    Application::Interface app_;

    std::unique_ptr<RoundState> state_;

    std::deque<TrailThing> trails_;

    std::vector<PlayerThing> players_;
    std::vector<PlayerThing>::iterator lastAlive_;

    uint32_t scoreVictoryGoal_;

    BorderThing border_;

    sf::Clock moveTimer_; // for calculating per-pixel movement speed and turn angles

    std::vector<PowerupThing> powerups_;
    sf::Clock powerupSpawnTimer_;
    sf::Time timeUntilNextPowerupSpawn_;

    struct RoundState {
        virtual void onEnter() = 0;
        virtual void onSpacebar() = 0;
        virtual void onEscape() = 0;
        virtual void onTick() = 0;
    protected:
        explicit RoundState(StateGame& s) : gs{s} {}
        StateGame& gs;
    };

    static constexpr auto PRINT = "RoundState::{} ctor";

    struct RoundBegin : RoundState {
        RoundBegin(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnter(); }
        void onEnter() override;
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {}
    };

    struct Running : RoundState {
        Running(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnter(); }
        void onEnter() override;
        void onSpacebar() override;
        void onEscape() override {}
        void onTick() override;
    };

    struct Pause : RoundState {
        Pause(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnter(); }
        void onEnter() override {}
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };

    struct RoundEnd : RoundState {
        RoundEnd(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnter(); }
        void onEnter() override {}
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };

    struct GameEnd : RoundState {
        GameEnd(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnter(); }
        void onEnter() override;
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };
};
