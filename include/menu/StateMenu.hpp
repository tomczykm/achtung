#pragma once

#include "IGameState.hpp"

#include <optional>
#include <TGUI/TGUI.hpp>

#include "Application.hpp"

class StateMenu : public IGameState {
public:
    explicit StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override {}
    void render() override {}
private:
    void addPlayer();
    void loadGui();

    Application::Interface app_;
};