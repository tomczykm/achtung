#pragma once

#include "engine/Player.hpp"
#include "testable/InputSequence.hpp"

class PlayerTestable : public PlayerThing {
public:
    PlayerTestable(const InputSequence& inputs, const PlayerInfo& info, float radius, int vel, Timer::Ptr gapSwitchTimer):
        PlayerThing{info, radius, vel, gapSwitchTimer},
        inputs_{inputs}
    {}

    void setPosition(float x, float y) {
        PlayerThing::setPosition(x, y);
    }

    bool isKeyPressed(sf::Keyboard::Key k) const override {
        return inputs_.isKeyPressed(k);
    }

    sf::Vector2f getPosition() { return shape_.getPosition(); }
    void setDirection(double deg) { direction_ = deg; }
    float getRadius() { return shape_.getRadius(); }
    void setDead(bool dead) { dead_ = dead; }

    bool isOnBaseVelocity() { return vel_ == baseVel_; }

    std::size_t getTotalEffects() {
        std::size_t sum = 0;
        for (const auto&[effect, stack]: effectStacks_) {
            sum += stack.size();
        }
        return sum;
    }


    bool isRightAngled() const { return PlayerThing::isRightAngled(); }

private:
    const InputSequence& inputs_;
};
