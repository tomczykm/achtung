#pragma once

#include <SFML/Graphics.hpp>

#include "app/AssetManager.hpp"
#include "game/Player.hpp"
#include "game/BorderThing.hpp"
#include "game/PickMeUp.hpp"
#include "framework/Observer.hpp"

#include <cstdint>
#include <deque>
#include <map>
#include <vector>
#include <functional>

class Engine : public framework::Observable {
public:
    using Players = std::map<ProfileId, PlayerThing>;

    Engine(AssetManager&, const PlayerInfos&, int playAreaCorner, int playAreaSide);

    void input(const sf::Event&);
    void step(double deltaTime);
    void resetRound();

    bool checkVictoryGoal();
    std::vector<const sf::Drawable*> getDrawables();

private:
    void initializePlayers(const PlayerInfos&);

    bool checkCollisions(PlayerThing&);  // returns true if they died
    void awardPoints();
    bool victoryGoalAchieved();

    void createRandomPickMeUp();
    std::pair<PickMeUp::OnPickUp, AssetManager::Texture> getRandomPickMeUpEffect();
    void resetPickmeupSpawnTimer();

    template <typename PlayerUnaryOp> PickMeUp::OnPickUp makeSelfEffect(PlayerUnaryOp);
    template <typename PlayerUnaryOp> PickMeUp::OnPickUp makeOpponentEffect(PlayerUnaryOp);

    void addVelocityChange(PlayerThing&, int velChange, int turnAngleChange, sf::Time);
    void addRightAngleMovement(PlayerThing&, sf::Time);
    void addControlSwap(PlayerThing&, sf::Time);
    void addMassPowerups();

    AssetManager& assets_;

    int playAreaCornerOffset_;
    int playAreaSideLength_;

    float pickMeUpRadius_;

    std::deque<TrailThing> trails_;

    Players players_;

    uint32_t scoreVictoryGoal_;

    BorderThing border_;

    std::vector<PickMeUp> pickmeups_;
    sf::Clock pickmeupSpawnTimer_;
    sf::Time timeUntilNextPickmeupSpawn_;

    std::optional<TimedEffect> massPowerups_;
};

struct RoundEndEvent : framework::IEvent {};

struct GameEndEvent : framework::IEvent {};

struct PointsAwardedEvent : framework::IEvent {
    PointsAwardedEvent(const Engine::Players&p): players{p} {}
    const Engine::Players& players;
};
