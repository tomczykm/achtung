#include "game/PlayerThing.hpp"

#include <cmath>

#include "Global.hpp"

PlayerThing::PlayerThing(std::string_view n, SDL_Scancode left, SDL_Scancode right):
    name_(n),
    leftKey_(left),
    rightKey_(right)
{}

void PlayerThing::input() {
    const auto keyStates = SDL_GetKeyboardState(nullptr);
    turnL_ = keyStates[ leftKey_ ] && !keyStates[ rightKey_ ];
    turnR_ = keyStates[ rightKey_ ];
}

void PlayerThing::move(double timeStep) {
    if (turnL_) direction_ -= timeStep * TURN_DEG;
    else if (turnR_) direction_ += timeStep * TURN_DEG;

    if (direction_ < 0) direction_ += 360;
    else if (direction_ >= 360) direction_ -= 360;

    if (chrono::steady_clock::now() >= switchTime_) {
        gapSwitch();
    }

    xPos_ += timeStep * vel_ * sin(-(M_PI/180)*direction_);
    yPos_ += timeStep * vel_ * cos(-(M_PI/180)*direction_);
}

// places a player randomly on the playfield and moves them a couple steps
// so that the player can tell the direction they're going to move
void PlayerThing::newRoundSetup(int xmin, int xmax, int ymin, int ymax) {
    dead_ = false;
    xPos_ = randomInt(xmin, xmax);
    yPos_ = randomInt(ymin, ymax);
    direction_ = std::rand()%360;

    // todo: move the player

    gap_ = true;
    gapSwitch();
}

void PlayerThing::createTrail(std::deque <TrailThing> &trails) const {
    if (!gap_) {
        trails.emplace_front(xPos_, yPos_, direction_, radius_);
    }
}

SDL_Rect PlayerThing::getRenderRect() const {
    return {static_cast<int>(xPos_-radius_/2), static_cast<int>(yPos_-radius_/2),
        radius_, radius_};
}

void PlayerThing::gapSwitch() {
    gap_ = !gap_;
    switchTime_ = chrono::steady_clock::now() + (gap_ ? GAP_TIME : chrono::milliseconds(randomInt( 1800, 3900 )));
}

bool PlayerThing::checkCollision(const TrailThing &o) const {
    return false;
}
