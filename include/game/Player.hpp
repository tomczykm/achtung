#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "Log.hpp"
#include "game/TrailThing.hpp"

struct PlayerInfo {
    std::string name;
    sf::Keyboard::Key left, right;
    sf::Color color;
};

class PlayerThing {
public:
    explicit PlayerThing(const PlayerInfo&);

    void move(double timeStep);

    void newRoundSetup(int xmin, int xmax, int ymin, int ymax, std::deque<TrailThing>&);
    void createTrail(std::deque<TrailThing>&) const;

    const sf::CircleShape& getShape() const { return shape_; }

    bool checkCollision(const sf::Shape&) const;

    void kill() { dead_ = true; }
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }

private:
    void gapSwitch();

    PlayerInfo info_;

    sf::CircleShape shape_;
    double direction_ = 0.0; // in degrees
    int vel_ = 100; // pixels per seconds

    // int score_ = 0;

    bool dead_ = false;

    // gap logic
    bool gap_ = false;
    std::chrono::steady_clock::time_point switchTime_ = {};
};
