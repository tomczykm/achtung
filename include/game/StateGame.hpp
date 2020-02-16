#pragma once

#include "Application.hpp"
#include "IGameState.hpp"
#include "game/Player.hpp"
#include "game/BorderThing.hpp"

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

    std::vector<PlayerThing> initializePlayers(const std::vector<PlayerInfo>&);
    void loadGui();
    void sortScoreList();

    template <typename State>
    void changeState() { state_ = std::make_unique<State>(*this); }

    Application::Interface app_;

    std::unique_ptr<RoundState> state_;

    std::deque<TrailThing> trails_;

    std::vector<PlayerThing> players_;
    std::vector<PlayerThing>::iterator lastAlive_;

    BorderThing border_;

    // for calculating per-pixel movement speed and turn angles
    sf::Clock moveTimer_;

    struct RoundState {
        virtual void onTick() = 0;
        virtual void onSpacebar() = 0;
        virtual void onEscape() = 0;
    protected:
        explicit RoundState(StateGame& s) : gs{s} {}
        StateGame& gs;
    };

    static constexpr auto PRINT = "RoundState::{} ctor";

    struct RoundBegin : RoundState {
        RoundBegin(StateGame& s);
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {}
    };

    struct Running : RoundState {
        Running(StateGame& s);
        void onSpacebar() override;
        void onEscape() override {}
        void onTick() override;
    };

    struct Pause : RoundState {
        Pause(StateGame& s) : RoundState{s} { print::info(PRINT, __func__); }
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };

    struct RoundEnd : RoundState {
        RoundEnd(StateGame& s) : RoundState{s} { print::info(PRINT, __func__); }
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };

    struct GameEnd : RoundState {
        GameEnd(StateGame& s);
        void onSpacebar() override;
        void onEscape() override;
        void onTick() override {};
    };
};
