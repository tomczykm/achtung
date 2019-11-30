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
    PlayerThing(std::string_view name/*, SDL_Scancode left, SDL_Scancode right*/);

    void input();
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

    // movement
    sf::CircleShape shape_;
    double direction_ = 0.0; // in degrees
    int vel_ = 100; // pixels per seconds

    // int score_ = 0;

    bool dead_ = false;

    // SDL_Scancode leftKey_, rightKey_;

    // gap logic
    static constexpr std::chrono::milliseconds GAP_TIME{170};
    bool gap_ = false;
    std::chrono::steady_clock::time_point switchTime_ = {};
};
