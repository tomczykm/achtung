#pragma once

#include "app/Application.hpp"
#include "app/IGameState.hpp"
#include "game/Player.hpp"
#include "game/BorderThing.hpp"
#include "game/PickMeUp.hpp"

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
    using PlayerIt = std::vector<PlayerThing>::iterator;

    void initializePlayers(const std::vector<PlayerInfo>&);
    void loadGui();
    void sortScoreList();

    bool checkCollisions(PlayerThing& player);  // returns true if they died
    void awardPoints();
    bool victoryGoalAchieved();

    PlayerIt getPlayer(std::string_view);

    void createRandomPickMeUp();
    std::pair<PickMeUp::OnPickUp, AssetManager::Texture> getRandomPickMeUpEffect();
    void resetPickmeupSpawnTimer();

    template <typename OnPickUp>
    PickMeUp::OnPickUp makeSelfEffect(OnPickUp);
    template <typename OnPickUp>
    PickMeUp::OnPickUp makeOpponentEffect(OnPickUp);

    void addHaste(PlayerIt p, sf::Time);
    void addSlow(PlayerIt p, sf::Time);

    template <typename State>
    void changeState() { state_ = std::make_unique<State>(*this); }

    Application::Interface app_;

    float playAreaSideLength_;
    float pickMeUpRadius_;

    std::unique_ptr<RoundState> state_;

    std::deque<TrailThing> trails_;

    std::vector<PlayerThing> players_;
    PlayerIt lastAlive_;

    uint32_t scoreVictoryGoal_;

    BorderThing border_;

    sf::Clock moveTimer_; // for calculating per-pixel movement speed and turn angles

    std::vector<PickMeUp> pickmeups_;
    sf::Clock pickmeupSpawnTimer_;
    sf::Time timeUntilNextPickmeupSpawn_;

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
