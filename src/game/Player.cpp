#include "game/Player.hpp"

#include <cmath>
#include <algorithm>

#include "app/Log.hpp"
#include "app/Utils.hpp"

namespace chrono = std::chrono;

namespace
{
constexpr std::chrono::milliseconds GAP_TIME{240};
}

PlayerThing::PlayerThing(const PlayerInfo& info, tgui::Label::Ptr scoreLabel, float radius):
    info_{info},
    shape_{radius},
    scoreLabel_{scoreLabel}
{
    shape_.setOrigin(radius, radius);
    shape_.setFillColor(sf::Color::Yellow);
}

void PlayerThing::move(double timeStep) {
    endExpiredEffects();

    if (sf::Keyboard::isKeyPressed(info_.right)) {
        direction_ += timeStep * turnDegrees_;
    } else if (sf::Keyboard::isKeyPressed(info_.left)) {
        direction_ -= timeStep * turnDegrees_;
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

void PlayerThing::newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>& trails) {
    effects_.clear();
    dead_ = false;
    direction_ = xor_rand::next(0, 360);
    shape_.setPosition(xPos, yPos);

    // a little trick to set switch time without code duplication
    gap_ = true;
    gapSwitch();

    // move a little bit forward
    for (int i = 0; i < 4; i++) {
        createTrail(trails);
        auto [xPos, yPos] = shape_.getPosition();
        shape_.setPosition(
            xPos + (2 * sin(-(M_PI/180)*direction_)),
            yPos + (2 * cos(-(M_PI/180)*direction_))
        );
    }
}

void PlayerThing::createTrail(std::deque<TrailThing>& trails) const {
    if (!gap_) {
        auto [xPos, yPos] = shape_.getPosition();
        trails.emplace_front(xPos, yPos, direction_, shape_.getRadius()*2, info_.color);
    }
}

void PlayerThing::gapSwitch() {
    gap_ = !gap_;
    switchTime_ = chrono::steady_clock::now() + (gap_ ? GAP_TIME : chrono::milliseconds(xor_rand::next( 1800, 3900 )));
}

bool PlayerThing::checkCollision(const sf::Shape &o) const {
    return shape_.getGlobalBounds().intersects(o.getGlobalBounds());
}

void PlayerThing::addPoint() {
    score_ += 1;
    if (scoreLabel_) {
        scoreLabel_->setText(std::to_string(score_));
    }
}

void PlayerThing::kill() {
    if (!dead_) {
        print::info("kill {}", info_.name);
        dead_ = true;
    }
}

void PlayerThing::endExpiredEffects() {
    effects_.erase(std::remove_if(effects_.begin(), effects_.end(), [] (const auto& effect) {
        return effect.isExpired();
    }), effects_.end());
}
