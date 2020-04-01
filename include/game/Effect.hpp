#pragma once

#include <functional>

#include "game/Timer.hpp"

class TimedEffect {
public:
    using Action = std::function<void()>;

    TimedEffect(Timer::Ptr timer, const Action& onExpire):
        timer_{timer},
        onExpire_{onExpire}
    {}
    ~TimedEffect() { onExpire_(); }

    void extendBy(Ticks t) {
        timer_->extend(t);
    }

    bool isExpired() const {
        return timer_->isExpired();
    }
private:
    Timer::Ptr timer_;
    Action onExpire_;
};
