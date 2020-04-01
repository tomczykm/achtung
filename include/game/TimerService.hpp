#pragma once

#include <memory>
#include <vector>

#include <SFML/System/Time.hpp>

#include "game/Timer.hpp"

class TimerService {
public:
    TimerService(int tickrate): tickrate_{tickrate} {}

    void advanceAll();
    Timer::Ptr makeTimer(sf::Time time);

    Ticks timeToTicks(sf::Time time) { return time.asMilliseconds() * tickrate_ / 1000; }

private:
    int tickrate_;
    std::vector<std::weak_ptr<Timer>> timers_;

};
