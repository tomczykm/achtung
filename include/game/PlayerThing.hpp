#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "Log.hpp"
#include "game/TrailThing.hpp"

class PlayerThing {
public:
    PlayerThing(std::string_view name, sf::Keyboard::Key left, sf::Keyboard::Key right);

    void move(double timeStep);

    void newRoundSetup(int xmin, int xmax, int ymin, int ymax);
    void createTrail(std::deque<TrailThing>&) const;

    const sf::CircleShape& getShape() const { return shape_; }

    bool checkCollision(const TrailThing&) const;

    void kill() { dead_ = true; }
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }

private:
    void gapSwitch();

    std::string name_;

    sf::CircleShape shape_;
    double direction_ = 0.0; // in degrees
    int vel_ = 100; // pixels per seconds

    // int score_ = 0;

    bool dead_ = false;

    sf::Keyboard::Key leftKey_, rightKey_;
    sf::Color color_;

    // gap logic
    bool gap_ = false;
    std::chrono::steady_clock::time_point switchTime_ = {};
};
