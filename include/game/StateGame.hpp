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
    StateGame(const Application::Interface&, const PlayerInfos&);
    ~StateGame();

    void input(const sf::Event&) override;
    void tick(double deltaTime) override;
    void render() override;

private:
    class RoundState;

    void initializePlayers(const PlayerInfos&);
    void loadGui();
    void sortScoreList();

    bool checkCollisions(PlayerThing& player);  // returns true if they died
    void awardPoints();
    bool victoryGoalAchieved();

    void createRandomPickMeUp();
    std::pair<PickMeUp::OnPickUp, AssetManager::Texture> getRandomPickMeUpEffect();
    void resetPickmeupSpawnTimer();

    template <typename PlayerUnaryOp>
    PickMeUp::OnPickUp makeSelfEffect(PlayerUnaryOp);
    template <typename PlayerUnaryOp>
    PickMeUp::OnPickUp makeOpponentEffect(PlayerUnaryOp);

    void addVelocityChange(PlayerThing&, int velChange, int turnAngleChange, sf::Time);
    void addRightAngleMovement(PlayerThing&, sf::Time);
    void addControlSwap(PlayerThing&, sf::Time);
    void addMassPowerups();

    template <typename State>
    void changeState() { state_ = std::make_unique<State>(*this); }

    Application::Interface app_;

    int playAreaCornerOffset_;
    int playAreaSideLength_;

    float pickMeUpRadius_;

    std::unique_ptr<RoundState> state_;

    std::deque<TrailThing> trails_;

    std::map<ProfileId, PlayerThing> players_;

    uint32_t scoreVictoryGoal_;

    BorderThing border_;

    std::vector<PickMeUp> pickmeups_;
    sf::Clock pickmeupSpawnTimer_;
    sf::Time timeUntilNextPickmeupSpawn_;

    std::optional<TimedEffect> massPowerups_;

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
        void onEnterState() override;
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

    struct GameEnd : RoundState {
        GameEnd(StateGame& s): RoundState{s} { print::info(PRINT, __func__); onEnterState(); }
        void onEnterState() override;
        void onSpacebar() override;
        void onEscape() override;
        void onTick(double) override {};
    };
};
