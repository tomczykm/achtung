#pragma once

#include "app/Application.hpp"
#include "app/IGameState.hpp"
#include "testable/EngineTestable.hpp"
#include "testable/InputSequence.hpp"

class StateViewer : public IGameState {
public:
    StateViewer(const Application::Interface&, int testcaseIndex);
    ~StateViewer();

    void input(const sf::Event&) override;
    void tick(double deltaTime) override;
    void render() override;

private:
    void setupState();
    void onFinished();

    Application::Interface app_;
    EngineTestable engine_;

    Ticks currentTick_ = 0;
    Ticks ticksToRun_;
    bool started_ = false;

    InputSequence inputs_;
};
