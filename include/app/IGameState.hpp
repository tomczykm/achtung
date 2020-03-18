#pragma once

namespace sf
{
class Event;
}

class IGameState {
public:
    virtual void tick(double deltaTime) = 0;
    virtual void render() = 0;
    virtual void input(const sf::Event&) = 0;

    virtual ~IGameState() = default;
};
