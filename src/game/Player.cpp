#include "game/Player.hpp"

#include <cmath>
#include <algorithm>

#include "app/Log.hpp"
#include "app/Utils.hpp"

PlayerThing::PlayerThing(const PlayerInfo& info, tgui::Panel::Ptr labels, float radius, int vel):
    info_{info},
    shape_{radius},
    recShape_{{radius*2, radius*2}},
    vel_{vel},
    scoreLabels_{labels}
{
    shape_.setOrigin(radius, radius);
    shape_.setFillColor(sf::Color::Yellow);

    recShape_.setOrigin(radius, radius);
    recShape_.setFillColor(sf::Color::Yellow);
}

const sf::Shape& PlayerThing::getShape() const {
    if (rightAngleMovement_) {
        return recShape_;
    } else {
        return shape_;
    }
}

void PlayerThing::tick(double timeStep, std::deque<TrailThing>& trails) {
    endExpiredEffects();

    if (!rightAngleMovement_) {
        if (sf::Keyboard::isKeyPressed(info_.right)) {
            direction_ += timeStep * turnDegrees_;
        } else if (sf::Keyboard::isKeyPressed(info_.left)) {
            direction_ -= timeStep * turnDegrees_;
        }
    }

    if (direction_ < 0) direction_ += 360;
    else if (direction_ >= 360) direction_ -= 360;

    if (gapSwitchTimer_.getElapsedTime() >= gapSwitchDuration_) {
        gapSwitch();
    }

    move(timeStep, trails);
}

void PlayerThing::move(double timeStep, std::deque<TrailThing>& trails) {
    auto [oldX, oldY] = shape_.getPosition();
    setPosition(
        oldX + (timeStep * vel_ * sin(-(M_PI/180)*direction_)),
        oldY + (timeStep * vel_ * cos(-(M_PI/180)*direction_))
    );

    if (!gap_) {
        const auto newPos = shape_.getPosition();
        const auto numSegments = distance(newPos, {oldX, oldY}) / (TrailThing::height-1);
        for (auto i = 0u; i < numSegments; ++i) {
            trails.emplace_front(oldX, oldY, direction_, shape_.getRadius()*2, info_.color);
            oldX += ((TrailThing::height-1) * sin(-(M_PI/180)*direction_));
            oldY += ((TrailThing::height-1) * cos(-(M_PI/180)*direction_));
        }
    }
}

void PlayerThing::input(const sf::Event& e) {
    if (rightAngleMovement_ && e.type == sf::Event::KeyPressed) {
        if (e.key.code == info_.right) {
            direction_ += 90;
        } else if (e.key.code == info_.left) {
            direction_ -= 90;
        }
    }
}

void PlayerThing::newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>& trails) {
    effects_.clear();
    dead_ = false;
    direction_ = xor_rand::next(0, 360);
    shape_.setPosition(xPos, yPos);

    // a little trick to set switch time without code duplication
    gap_ = true;
    gapSwitch();

    move(0.1, trails);
}

void PlayerThing::setRightAngleMovement(bool v) {
    rightAngleMovement_ = v;
    recShape_.setRotation(direction_);
}

void PlayerThing::swapControls() {
    std::swap(info_.left, info_.right);
    if (shape_.getFillColor() == sf::Color::Blue) {
        shape_.setFillColor(sf::Color::Yellow);
        recShape_.setFillColor(sf::Color::Yellow);
    } else {
        shape_.setFillColor(sf::Color::Blue);
        recShape_.setFillColor(sf::Color::Blue);
    }

}

void PlayerThing::gapSwitch() {
    gap_ = !gap_;
    const auto gapTime = sf::seconds(6 * shape_.getRadius() / vel_);
    gapSwitchDuration_ = gap_ ? gapTime : sf::milliseconds(xor_rand::next(1400, 7000));
    gapSwitchTimer_.restart();
}

bool PlayerThing::checkCollision(const sf::Shape &o) const {
    return shape_.getGlobalBounds().intersects(o.getGlobalBounds());
}

void PlayerThing::addPoint() {
    score_ += 1;
    std::static_pointer_cast<tgui::Label>(scoreLabels_->get("Score"))
        ->setText(std::to_string(score_));
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

void PlayerThing::setPosition(float x, float y) {
    recShape_.setPosition(x, y);
    shape_.setPosition(x, y);
}
