#include "game/PlayerThing.hpp"

#include <cmath>

#include "Log.hpp"
#include "Utils.hpp"

namespace chrono = std::chrono;

namespace
{
constexpr int BASE_RADIUS = 3;
constexpr double TURN_DEG = 160;
constexpr std::chrono::milliseconds GAP_TIME{240};
}

PlayerThing::PlayerThing(std::string_view n, sf::Keyboard::Key left, sf::Keyboard::Key right):
    name_(n),
    shape_(BASE_RADIUS),
    leftKey_(left),
    rightKey_(right),
    color_(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255))
{
    shape_.setOrigin(BASE_RADIUS, BASE_RADIUS);
    shape_.setFillColor(sf::Color::Yellow);
}

void PlayerThing::move(double timeStep) {
    if (sf::Keyboard::isKeyPressed(rightKey_)) {
        direction_ += timeStep * TURN_DEG;
    } else if (sf::Keyboard::isKeyPressed(leftKey_)) {
        direction_ -= timeStep * TURN_DEG;
    }

    if (direction_ < 0) direction_ += 360;
    else if (direction_ >= 360) direction_ -= 360;

    if (chrono::steady_clock::now() >= switchTime_) {
        gapSwitch();
    }

    auto [xPos, yPos] = shape_.getPosition();
    shape_.setPosition(
        xPos + (timeStep * vel_ * sin(-(M_PI/180)*direction_)),
        yPos + (timeStep * vel_ * cos(-(M_PI/180)*direction_))
    );
}

// places a player randomly on the playfield and moves them a couple steps
// so that the player can tell the direction they're going to move
void PlayerThing::newRoundSetup(int xmin, int xmax, int ymin, int ymax, std::deque<TrailThing>& trails) {
    dead_ = false;
    direction_ = std::rand()%360;
    shape_.setPosition(
        randomInt(xmin, xmax),
        randomInt(ymin, ymax)
    );

    gap_ = true;
    gapSwitch();

    // move a little bit
    for (int i = 0; i < 4; i++) {
        createTrail(trails);
        auto [xPos, yPos] = shape_.getPosition();
        shape_.setPosition(
            xPos + (0.02 * vel_ * sin(-(M_PI/180)*direction_)),
            yPos + (0.02 * vel_ * cos(-(M_PI/180)*direction_))
        );
    }
}

void PlayerThing::createTrail(std::deque<TrailThing>& trails) const {
    if (!gap_) {
        auto [xPos, yPos] = shape_.getPosition();
        trails.emplace_front(xPos, yPos, direction_, shape_.getRadius()*2, color_);
    }
}

void PlayerThing::gapSwitch() {
    gap_ = !gap_;
    switchTime_ = chrono::steady_clock::now() + (gap_ ? GAP_TIME : chrono::milliseconds(randomInt( 1800, 3900 )));
}

bool PlayerThing::checkCollision(const TrailThing &o) const {
    return false;
}
