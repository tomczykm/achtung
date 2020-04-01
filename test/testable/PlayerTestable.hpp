#pragma once

#include "engine/Player.hpp"
#include "testable/InputSequence.hpp"

class PlayerTestable : public PlayerThing {
public:
    PlayerTestable(const InputSequence& inputs, float radius, int vel, Timer::Ptr gapSwitchTimer):
        PlayerThing{
            PlayerInfo{"test", sf::Keyboard::Q, sf::Keyboard::W, sf::Color::White},
            radius, vel, gapSwitchTimer
        },
        inputs_{inputs}
    {}

    void setPosition(float x, float y) {
        PlayerThing::setPosition(x, y);
    }

    bool isKeyPressed(sf::Keyboard::Key k) override {
        return inputs_.isKeyPressed(k);
    }

    sf::Vector2f getPosition() { return shape_.getPosition(); }
    void setDirection(double deg) { direction_ = deg; }
    float getRadius() { return shape_.getRadius(); }
    void setDead(bool dead) { dead_ = dead; }

private:
    const InputSequence& inputs_;
};
