#pragma once

#include "IGameState.hpp"
#include "Application.hpp"

class StateSettings: public IGameState {
public:
    StateSettings(Application &app);
    ~StateSettings();

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
    Application &app_;
};
