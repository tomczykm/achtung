#pragma once

enum class GameState {
    null,
    test,
    settings,
    quit
};

class BaseGameState {
public:
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual void input() = 0;
    virtual void polledInput() = 0;

    virtual ~BaseGameState() {}
    static void setNextState(GameState newState);
    static void changeState();
};
