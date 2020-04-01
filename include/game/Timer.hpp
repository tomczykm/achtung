#pragma once

#include <cstdint>
#include <memory>

using Ticks = std::int64_t;

class Timer {
public:
    using Ptr = std::shared_ptr<Timer>;

    void start() { paused_ = false; }
    void pause() { paused_ = true; }

    bool isExpired() { return remaining_ <= 0; }

    void advance() { if (!paused_) --remaining_; }

    void extend(Ticks t) { remaining_ += t; }
    void reset(Ticks t) { remaining_ = t; }

private:
    friend class TimerService;
    Timer(Ticks duration): remaining_{duration} {}

    bool paused_ = false;
    Ticks remaining_;
};
