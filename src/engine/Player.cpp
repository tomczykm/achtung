#include "engine/Player.hpp"

#include <cmath>
#include <algorithm>

#include "app/Log.hpp"
#include "app/Utils.hpp"

namespace {

const std::set<PlayerEffect> appliedOnEveryStack = {
    PlayerEffect::Haste, PlayerEffect::Slow,
    PlayerEffect::Enlarge, PlayerEffect::Shrink
};

const std::set<PlayerEffect> appliedOnFirstStack = {
    PlayerEffect::RightAngled, PlayerEffect::SwapControl, PlayerEffect::Warp
};

constexpr auto playerToGameAreaSizeRatio = 253.334f;
constexpr auto playerSpeedToGameAreaSizeRatio = 7.6f;

}

PlayerThing::PlayerThing(const PlayerInfo& info, int playAreaCorner, int playAreaSide, Timer::Ptr gapSwitchTimer):
    info_{info},
    playAreaCorner_{playAreaCorner},
    playAreaSide_{playAreaSide},
    baseVel_{playAreaSide_ / playerSpeedToGameAreaSizeRatio},
    baseRadius_{playAreaSide_ / playerToGameAreaSizeRatio},
    shape_{baseRadius_},
    recShape_{{baseRadius_*2, baseRadius_*2}},
    vel_{baseVel_},
    gapSwitchTimer_{gapSwitchTimer}
{
    shape_.setOrigin(baseRadius_, baseRadius_);
    shape_.setFillColor(sf::Color::Yellow);

    recShape_.setOrigin(baseRadius_, baseRadius_);
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

    auto [newX, newY] = shape_.getPosition();
    if (!gap_) {
        const auto numSegments = distance({newX, newY}, {oldX, oldY}) / (TrailThing::height-1);
        for (auto i = 0u; i < numSegments; ++i) {
            trails.emplace_front(oldX, oldY, direction_, shape_.getRadius()*2, info_.color);
            oldX += ((TrailThing::height-1) * sin(-(M_PI/180)*direction_));
            oldY += ((TrailThing::height-1) * cos(-(M_PI/180)*direction_));
        }
    }

    // warps
    if (newX < playAreaCorner_) {
        setPosition(newX + playAreaSide_  - 4, newY);
    }
    if (newY < playAreaCorner_) {
        setPosition(newX, newY + playAreaSide_ - 4);
    }
    if (newX > playAreaSide_ + playAreaCorner_) {
        setPosition(newX - playAreaSide_ + 4, newY);
    }
    if (newY > playAreaSide_+ playAreaCorner_) {
        setPosition(newX, newY - playAreaSide_ + 4);
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

void PlayerThing::setAlpha(std::uint8_t alpha) {
    const auto newColor = sf::Color{0xff, 0xff, 0x00, alpha};
    shape_.setFillColor(newColor);
    recShape_.setFillColor(newColor);
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
            updateRadius();
            break;
        case PlayerEffect::Shrink:
            updateRadius();
            break;
        case PlayerEffect::Warp:
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
            updateRadius();
            break;
        case PlayerEffect::Shrink:
            updateRadius();
            break;
        case PlayerEffect::Warp:
            setAlpha(255u);
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

    return velTier >= 0 ?
        baseVel_ * (velTier+1) :
        baseVel_ / std::pow(2, -velTier);
}

void PlayerThing::updateRadius() {
    int sizeTier = 0;
    if (auto it = effectStacks_.find(PlayerEffect::Enlarge); it != effectStacks_.end()) {
        sizeTier += it->second.size();
    }
    if (auto it = effectStacks_.find(PlayerEffect::Shrink); it != effectStacks_.end()) {
        sizeTier -= it->second.size();
    }

    const auto newRadius = sizeTier >= 0 ?
        baseRadius_ * (sizeTier+1) :
        baseRadius_ * std::pow(2/3.f, -sizeTier);

    shape_.setRadius(newRadius);
    shape_.setOrigin(newRadius, newRadius);
    recShape_.setSize({newRadius*2, newRadius*2});
    recShape_.setOrigin(newRadius, newRadius);
}
