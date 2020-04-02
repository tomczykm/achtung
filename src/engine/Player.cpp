#include "engine/Player.hpp"

#include <cmath>
#include <algorithm>

#include "app/Log.hpp"
#include "app/Utils.hpp"

namespace {

std::set<PlayerEffect> appliedOnEveryStack = {
    PlayerEffect::Haste, PlayerEffect::Slow
};

std::set<PlayerEffect> appliedOnFirstStack = {
    PlayerEffect::RightAngled, PlayerEffect::SwapControl
};

}

PlayerThing::PlayerThing(const PlayerInfo& info, float radius, int vel, Timer::Ptr gapSwitchTimer):
    info_{info},
    shape_{radius},
    recShape_{{radius*2, radius*2}},
    baseVel_{vel},
    vel_{baseVel_},
    gapSwitchTimer_{gapSwitchTimer}
{
    shape_.setOrigin(radius, radius);
    shape_.setFillColor(sf::Color::Yellow);

    recShape_.setOrigin(radius, radius);
    recShape_.setFillColor(sf::Color::Yellow);
}

const sf::Shape& PlayerThing::getShape() const {
    if (isRightAngled()) {
        return recShape_;
    } else {
        return shape_;
    }
}

void PlayerThing::step(double timeStep, std::deque<TrailThing>& trails) {
    endExpiredEffects();

    if (not isRightAngled()) {
        if (isKeyPressed(info_.right)) {
            direction_ += timeStep * turnDegrees_;
        } else if (isKeyPressed(info_.left)) {
            direction_ -= timeStep * turnDegrees_;
        }
    }

    if (direction_ < 0) direction_ += 360;
    else if (direction_ >= 360) direction_ -= 360;

    if (gapSwitchTimer_->isExpired()) {
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
    if (isRightAngled() && e.type == sf::Event::KeyPressed) {
        if (e.key.code == info_.right) {
            direction_ += 90;
        } else if (e.key.code == info_.left) {
            direction_ -= 90;
        }
    }
}

void PlayerThing::newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>& trails) {
    clearAllEffects();
    dead_ = false;
    direction_ = xor_rand::next(0, 360);
    shape_.setPosition(xPos, yPos);

    // a little trick to set switch time without code duplication
    gap_ = true;
    gapSwitch();

    move(0.125, trails);
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
    const auto gapSwitchDuration = gap_ ? gapTime : sf::milliseconds(xor_rand::next(1400, 7000));
    gapSwitchTimer_->reset(gapSwitchDuration.asMilliseconds() / 1000.f * 140); // todo: hardcoded 140 tickrate
}

bool PlayerThing::checkCollision(const sf::Shape &o) const {
    if (gap_) return false;
    return shape_.getGlobalBounds().intersects(o.getGlobalBounds());
}

void PlayerThing::addPoint() {
    score_ += 1;
}

void PlayerThing::kill() {
    if (!dead_) {
        dead_ = true;
    }
}

void PlayerThing::endExpiredEffects() {
    for (auto&[e, stack] : effectStacks_) {
        auto sizeBefore = stack.size();
        stack.erase(std::remove_if(stack.begin(), stack.end(), [] (auto& t) {
            return t->isExpired();
        }), stack.end());
        auto removedTotal = sizeBefore - stack.size();
        if (removedTotal > 0) {
            if (appliedOnEveryStack.find(e) != appliedOnEveryStack.end()) {
                for (std::size_t i = 0 ; i < removedTotal ; ++i) {
                    revertEffect(e);
                }
            }
            if (appliedOnFirstStack.find(e) != appliedOnFirstStack.end() &&
                stack.size() == 0) {
                revertEffect(e);
            }
        }
    }
}

void PlayerThing::setPosition(float x, float y) {
    recShape_.setPosition(x, y);
    shape_.setPosition(x, y);
}

void PlayerThing::addEffectStack(PlayerEffect e, Timer::Ptr t) {
    auto it = effectStacks_.find(e);
    if (it == effectStacks_.end()) {
        effectStacks_.emplace(e, std::vector<Timer::Ptr>{t});
    } else {
        it->second.push_back(t);
    }

    if (appliedOnEveryStack.find(e) != appliedOnEveryStack.end()) {
        applyEffect(e);
    }

    if (effectStacks_[e].size() == 1 &&
        appliedOnFirstStack.find(e) != appliedOnFirstStack.end()) {
            applyEffect(e);
    }
}

int PlayerThing::getNumEffectStacks(PlayerEffect e) const {
    auto it = effectStacks_.find(e);
    if (it == effectStacks_.end()) return 0;
    return it->second.size();
}

void PlayerThing::applyEffect(PlayerEffect e) {
    print::info("{}: {} on {}", __func__, e, info_.name);
    switch (e) {
        case PlayerEffect::Haste:
            vel_ = calculateCurrentVelocity();
            break;
        case PlayerEffect::Slow:
            vel_ = calculateCurrentVelocity();
            break;
        case PlayerEffect::SwapControl:
            swapControls();
            break;
        case PlayerEffect::RightAngled:
            recShape_.setRotation(direction_);
            break;
        case PlayerEffect::Enlarge:
            // todo
            break;
        case PlayerEffect::Reduce:
            // todo
            break;
        case PlayerEffect::Wrap:
            // todo
            break;
        case PlayerEffect::NoTrails:
            // todo
            break;
    }
}

void PlayerThing::revertEffect(PlayerEffect e) {
    print::info("{}: {} on {}", __func__, e, info_.name);
    switch (e) {
        case PlayerEffect::Haste:
            vel_ = calculateCurrentVelocity();
            break;
        case PlayerEffect::Slow:
            vel_ = calculateCurrentVelocity();
            break;
        case PlayerEffect::SwapControl:
            swapControls();
            break;
        case PlayerEffect::RightAngled:
            break;
        case PlayerEffect::Enlarge:
            // todo
            break;
        case PlayerEffect::Reduce:
            // todo
            break;
        case PlayerEffect::Wrap:
            // todo
            break;
        case PlayerEffect::NoTrails:
            // todo
            break;
    }
}

void PlayerThing::clearAllEffects() {
    // basically, expire them all and clear expired.
    for (auto&[e, stack]: effectStacks_) {
        for(auto& t: stack) {
            t->reset(0);
        }
    }
    endExpiredEffects();
}

int PlayerThing::calculateCurrentVelocity() const {
    int velTier = 0;
    if (auto it = effectStacks_.find(PlayerEffect::Haste); it != effectStacks_.end()) {
        velTier += it->second.size();
    }
    if (auto it = effectStacks_.find(PlayerEffect::Slow); it != effectStacks_.end()) {
        velTier -= it->second.size();
    }

    if (velTier >= 0) {
        return baseVel_ * (velTier+1);
    } else {
        return baseVel_ / std::pow(2, -velTier);
    }
}
