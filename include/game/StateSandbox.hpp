#pragma once

#include "app/Application.hpp"
#include "app/IGameState.hpp"
#include "engine/EngineTestable.hpp"

class StateSandbox : public IGameState {
public:
    StateSandbox(Application::Interface&);
    ~StateSandbox();

    void input(const sf::Event&) override;
    void tick(double deltaTime) override;
    void render() override;

private:
    void setupState();
    void onFinished();

    Application::Interface app_;
    EngineTestable engine_;

    Ticks currentTick_ = 0;
    bool started_ = false;
};
