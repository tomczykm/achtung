#pragma once

#include <string>
#include <string_view>
#include <deque>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "app/ProfileManager.hpp"
#include "app/Log.hpp"
#include "engine/TrailThing.hpp"
#include "engine/Timer.hpp"

struct PlayerInfo {
    std::string name;
    sf::Keyboard::Key left, right;
    sf::Color color;
};

using PlayerInfos = std::map<ProfileId, PlayerInfo>;

enum class PlayerEffect {
    Haste,
    Slow,
    SwapControl,
    RightAngled,
    Enlarge,
    Shrink,
    Wrap,
    NoTrails
};

class PlayerThing {
public:
    using Score = uint32_t;

    PlayerThing(const PlayerInfo&, float radius, int vel, Timer::Ptr gapSwitchTimer);
    virtual ~PlayerThing() = default;

    void step(double timeStep, std::deque<TrailThing>& trails);
    void input(const sf::Event&);

    void newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>&);

    const sf::Shape& getShape() const;
    sf::Color getColor() const { return info_.color; }

    bool checkCollision(const sf::Shape&) const;

    void addPoint();
    Score getScore() const { return score_; }

    void kill();
    bool isDead() const { return dead_; }

    const std::string& name() const { return info_.name; }

    void addEffectStack(PlayerEffect, Timer::Ptr);

protected:
    void gapSwitch();
    void endExpiredEffects();

    void move(double timeStep, std::deque<TrailThing>& trails);
    virtual bool isKeyPressed(sf::Keyboard::Key k) const {
        return sf::Keyboard::isKeyPressed(k);
    }

    void setPosition(float x, float y);

    int getNumEffectStacks(PlayerEffect) const;

    void applyEffect(PlayerEffect);
    void revertEffect(PlayerEffect);

    void clearAllEffects();

    void swapControls();
    bool isRightAngled() const { return getNumEffectStacks(PlayerEffect::RightAngled) > 0; }

    int calculateCurrentVelocity() const;
    void updateRadius();

    PlayerInfo info_;

    sf::CircleShape shape_;
    sf::RectangleShape recShape_; // for right angle movement

    int baseVel_;
    int baseRadius_;
    double baseTurn_ = 130;

    double direction_ = 0.0; // in degrees
    int vel_; // pixels per seconds
    double turnDegrees_ = baseTurn_;

    Score score_ = 0;

    bool dead_ = false;

    bool gap_ = false;
    Timer::Ptr gapSwitchTimer_;

    std::map<PlayerEffect, std::vector<Timer::Ptr>> effectStacks_;
};
