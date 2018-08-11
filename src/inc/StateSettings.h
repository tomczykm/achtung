#pragma once

#include "BaseGameState.h"

class StateSettings: public BaseGameState {
public:
    StateSettings ();
    ~StateSettings ();

    void input() override;
    void logic() override;
    void render() override;
    void polledInput() override;
private:
};
