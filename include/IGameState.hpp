#pragma once

class IGameState {
public:
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual void input() = 0;
    virtual void polledInput() = 0;

    virtual ~IGameState() = default;
};
