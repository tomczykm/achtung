#pragma once

#include <string>
#include <deque>
#include <chrono>

#include <SDL.h>

#include "Thing.h"
#include "TrailThing.h"

using namespace std::chrono;

class PlayerThing : public Thing {
public:
    PlayerThing(std::string n, SDL_Scancode left, SDL_Scancode right);
    ~PlayerThing();

    void input();
    void move(double timeStep);

    void newRoundSetup(int xmin, int xmax, int ymin, int ymax);
    void createTrail(std::deque <TrailThing> &trails) const;

    SDL_Rect getRenderRect() const;
    int getRadius() const { return radius_; }

    bool checkCollision(const TrailThing &o) const;

    void die() { dead_ = true; }
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }
private:
    std::string name_;

    double direction_; //in degrees
    bool turnR_, turnL_;

    static const double TURN_DEG;
    int vel_; //pixels per seconds - depends on the size of the playfield

    int radius_; //for thickness powerups
    int score_;

    bool dead_;

    SDL_Scancode leftKey_, rightKey_;

    //gap logic
    static const milliseconds GAP_TIME;
    bool gap_;
    steady_clock::time_point switch_time_;
    void gapSwitch();
};
