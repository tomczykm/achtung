#pragma once

namespace sf
{
class Event;
}

class IGameState {
public:
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual void input(const sf::Event&) = 0;

    virtual ~IGameState() = default;
};
