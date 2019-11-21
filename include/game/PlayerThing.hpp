#pragma once

#include <string>
#include <string_view>
#include <deque>
#include <chrono>

#include <SDL.h>

#include "game/TrailThing.hpp"

namespace chrono = std::chrono;

class PlayerThing {
public:
    PlayerThing(std::string_view name, SDL_Scancode left, SDL_Scancode right);

    void input();
    void move(double timeStep);

    void newRoundSetup(int xmin, int xmax, int ymin, int ymax);
    void createTrail(std::deque<TrailThing>&) const;

    SDL_Rect getRenderRect() const;
    int getRadius() const { return radius_; }

    bool checkCollision(const TrailThing&) const;

    void die() { dead_ = true; }
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }

private:
    void gapSwitch();

    std::string name_;

    // movement
    double xPos_ = 0.0, yPos_ = 0.0;
    double direction_ = 0.0; // in degrees
    bool turnR_ = false, turnL_ = false;
    int vel_ = 100; // pixels per seconds
    static constexpr double TURN_DEG = 160;

    int radius_ = 7;
    // int score_ = 0;

    bool dead_ = false;

    SDL_Scancode leftKey_, rightKey_;

    // gap logic
    static constexpr chrono::milliseconds GAP_TIME{170};
    bool gap_ = false;
    chrono::steady_clock::time_point switchTime_ = {};
};
