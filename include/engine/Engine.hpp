#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "app/AssetManager.hpp"
#include "engine/Player.hpp"
#include "engine/BorderThing.hpp"
#include "engine/PickMeUp.hpp"
#include "engine/TimerService.hpp"
#include "framework/Observer.hpp"

#include <cstdint>
#include <deque>
#include <map>
#include <vector>
#include <functional>

class Engine : public framework::Observable {
public:
    using Players = std::map<ProfileId, std::unique_ptr<PlayerThing>>;
    using Pickmeups = std::vector<PickMeUp>;
    using Trails = std::deque<TrailThing>;

    Engine(IAssetManager&, const PlayerInfos&, int tickrate, int playAreaCorner, int playAreaSide);

    void input(const sf::Event&);
    void step(double deltaTime);
    void resetRound();

    bool checkVictoryGoal();
    std::vector<const sf::Drawable*> getDrawables();

protected:
    void initializePlayers(const PlayerInfos&);

    bool checkCollisions(PlayerThing&);  // returns true if they died
    void awardPoints();
    bool victoryGoalAchieved();

    void createRandomPickMeUp();
    std::pair<PickMeUp::OnPickUp, TextureType> makePickMeUpEffectAndTexture(PickUpType);
    void resetPickmeupSpawnTimer();

    template <typename PlayerUnaryOp> PickMeUp::OnPickUp makeSelfEffect(PlayerUnaryOp);
    template <typename PlayerUnaryOp> PickMeUp::OnPickUp makeOpponentEffect(PlayerUnaryOp);

    void addVelocityChange(PlayerThing&, int velChange, int turnAngleChange, sf::Time);
    void addRightAngleMovement(PlayerThing&, sf::Time);
    void addControlSwap(PlayerThing&, sf::Time);
    void addMassPowerups();

    IAssetManager& assets_;
    TimerService timerService_;

    int playAreaCornerOffset_;
    int playAreaSideLength_;

    float pickMeUpRadius_;

    Trails trails_;

    Players players_;

    uint32_t scoreVictoryGoal_;

    BorderThing border_;

    Pickmeups pickmeups_;
    Timer::Ptr pickmeupSpawnTimer_;

    std::optional<TimedEffect> massPowerups_;
};

struct RoundEndEvent : framework::IEvent {};

struct MatchEndEvent : framework::IEvent {};

struct PointsAwardedEvent : framework::IEvent {
    PointsAwardedEvent(const Engine::Players&p): players{p} {}
    const Engine::Players& players;
};
