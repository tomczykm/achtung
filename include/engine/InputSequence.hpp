#pragma once

#include <map>
#include <queue>
#include <utility>

#include <SFML/Graphics.hpp>

#include "engine/Timer.hpp"

class InputSequence {
public:
    bool isKeyPressed(sf::Keyboard::Key) const;
    bool pollEvent(sf::Event&) const;

    void advance() { ++currentTick_; }
    void clear() { keyPresses_.clear(); }

    void addKeyHold(sf::Keyboard::Key, Ticks start, Ticks end);
    void queueEvent(Ticks, const sf::Event&);

private:
    using Range = std::pair<Ticks, Ticks>;

    std::map<sf::Keyboard::Key, std::vector<Range>> keyPresses_;
    mutable std::map<Ticks, std::queue<sf::Event>> eventQueues_;

    Ticks currentTick_ = 0;
};
