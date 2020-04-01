#include "engine/TimerService.hpp"

void TimerService::advanceAll() {
    for (auto it = timers_.begin(); it != timers_.end();) {
        if (auto timer = it->lock(); timer) {
            timer->advance();
            ++it;
        } else {
            it = timers_.erase(it);
        }
    }
}

Timer::Ptr TimerService::makeTimer(sf::Time time) {
    auto ptr = Timer::Ptr(new Timer(timeToTicks(time)));
    timers_.emplace_back(ptr);
    return ptr;
}
