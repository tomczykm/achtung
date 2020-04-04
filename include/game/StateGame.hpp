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

    template <typename State, typename... Args>
    void changeState(Args&&... args) {
        state_ = std::make_unique<State>(*this, std::forward<Args>(args)...);
        state_->onEnterState();
    }

    void setInfoText(std::string_view);

    Application::Interface app_;

    std::unique_ptr<RoundState> state_;

    tgui::Label::Ptr infoLabel_;

    Engine engine_;

    struct RoundState {
        virtual void onEnterState() {};
        virtual void onSpacebar() {};
        virtual void onEscape() {};
        virtual void onTick(double) {};
        virtual ~RoundState() = default;
    protected:
        explicit RoundState(StateGame& s) : gs{s} {}
        StateGame& gs;
    };

    static constexpr auto PRINT = "RoundState::{} ctor";

    struct RoundBegin : RoundState {
        RoundBegin(StateGame& s): RoundState{s} { print::info(PRINT, __func__); }
        void onEnterState() override;
        void onSpacebar() override;
        void onEscape() override;
    };

    struct Running : RoundState {
        Running(StateGame& s): RoundState{s} { print::info(PRINT, __func__); }
        void onEnterState() override;
        void onSpacebar() override;
        void onTick(double deltaTime) override;
    };

    struct Pause : RoundState {
        Pause(StateGame& s): RoundState{s} { print::info(PRINT, __func__); }
        void onEnterState() override;
        void onSpacebar() override;
        void onEscape() override;
    };

    struct RoundEnd : RoundState {
        RoundEnd(StateGame& s, std::string_view winner): RoundState{s} {
            print::info(PRINT, __func__);
            onEnterState(winner);
        }
        void onEnterState(std::string_view winner);
        void onSpacebar() override;
        void onEscape() override;
    };

    struct MatchEnd : RoundState {
        MatchEnd(StateGame& s, std::string_view winner): RoundState{s} {
            print::info(PRINT, __func__);
            onEnterState(winner);
        }
        void onEnterState(std::string_view winner);
        void onSpacebar() override;
        void onEscape() override;
    };
};
