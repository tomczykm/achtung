#pragma once

#include "app/Application.hpp"
#include "app/IGameState.hpp"
#include "engine/Engine.hpp"

class StateGame: public IGameState {
public:
    StateGame(const Application::Interface&, const PlayerInfos&);
    ~StateGame();

    void input(const sf::Event&) override;
    void tick(double deltaTime) override;
    void render() override;

private:
    class RoundState;

    void loadGui(const PlayerInfos& infos);
    void prepareScoreLabels(const PlayerInfos&) ;
    void attachObservers();

    void updateScoreLabels(const Engine::Players&);

    template <typename State>
    void changeState() { state_ = std::make_unique<State>(*this); }

    Application::Interface app_;

    std::unique_ptr<RoundState> state_;

    Engine engine_;

    struct RoundState {
        virtual void onEnterState() = 0;
        virtual void onSpacebar() = 0;
        virtual void onEscape() = 0;
        virtual void onTick(double deltaTime) = 0;
    protected:
        explicit RoundState(StateGame& s) : gs{s} {}
        StateGame& gs;
    };

    static constexpr auto PRINT = "RoundState::{} ctor";

    struct RoundBegin : RoundState {
        RoundBegin(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override;
        void onSpacebar() override;
        void onEscape() override;
        void onTick(double) override {}
    };

    struct Running : RoundState {
        Running(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override {}
        void onSpacebar() override;
        void onEscape() override {}
        void onTick(double deltaTime) override;
    };

    struct Pause : RoundState {
        Pause(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override {}
        void onSpacebar() override;
        void onEscape() override;
        void onTick(double) override {};
    };

    struct RoundEnd : RoundState {
        RoundEnd(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override {}
        void onSpacebar() override;
        void onEscape() override;
        void onTick(double) override {};
    };

    struct MatchEnd : RoundState {
        MatchEnd(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override;
        void onSpacebar() override;
        void onEscape() override;
        void onTick(double) override {};
    };
};
