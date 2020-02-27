#pragma once

#include <functional>
#include <SFML/System/Clock.hpp>

class TimedEffect {
public:
    using Action = std::function<void()>;

    TimedEffect(sf::Time duration, const Action& onExpire):
        duration_{duration},
        onExpire_{onExpire}
    {}
    ~TimedEffect() { onExpire_(); }

    bool isExpired() const {
        return clock_.getElapsedTime() > duration_;
    }
private:
    sf::Clock clock_;
    sf::Time duration_;
    Action onExpire_;
};