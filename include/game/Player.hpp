#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "game/TrailThing.hpp"
#include "game/Effect.hpp"

struct PlayerInfo {
    std::string name;
    sf::Keyboard::Key left, right;
    sf::Color color;
};

class PlayerThing {
public:
    explicit PlayerThing(const PlayerInfo&, tgui::Label::Ptr scoreLabel, float radius);

    void move(double timeStep);

    void newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>&);
    void createTrail(std::deque<TrailThing>&) const;

    const sf::CircleShape& getShape() const { return shape_; }
    sf::Color getColor() const { return info_.color; }
    int getVelocity() const { return vel_; }

    bool checkCollision(const sf::Shape&) const;

    void addPoint();
    uint32_t getScore() const { return score_; }

    void kill();
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }

    const std::string& name() const { return info_.name; }

    void changeVelocity(int d) {
        vel_ += d;
    }
    void changeTurn(int d) {
        turnDegrees_ += d;
    }

    template <typename... Ts>
    void addTimedEffect(Ts&&... args) {
        effects_.emplace_back(std::forward<Ts>(args)...);
    }

private:
    void gapSwitch();
    void endExpiredEffects();

    PlayerInfo info_;

    sf::CircleShape shape_;
    double direction_ = 0.0; // in degrees
    int vel_ = 100; // pixels per seconds
    double turnDegrees_ = 160;

    uint32_t score_ = 0;
    tgui::Label::Ptr scoreLabel_;

    bool dead_ = false;

    // gap logic
    bool gap_ = false;
    std::chrono::steady_clock::time_point switchTime_ = {};

    std::vector<TimedEffect> effects_;
};
