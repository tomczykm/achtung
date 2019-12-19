#pragma once

#include <deque>

#include "Application.hpp"
#include "IGameState.hpp"
#include "game/PlayerThing.hpp"
#include "game/TrailThing.hpp"

class StateSandbox: public IGameState {
public:
    explicit StateSandbox(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override;
    void render() override;
private:
    Application::Interface app_;

    PlayerThing player_{"player", sf::Keyboard::Q, sf::Keyboard::W};
    std::deque<TrailThing> trails_;

    bool move_ = false;

    // for calculating per-pixel movement speed and turn angles
    sf::Clock moveTimer_;
};
