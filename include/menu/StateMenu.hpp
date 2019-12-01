#pragma once

#include "IGameState.hpp"

#include <optional>
#include <SFML/Graphics.hpp>

#include "Application.hpp"
#include "menu/Button.hpp"

class StateMenu : public IGameState {
public:
    StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override;
    void render() override;
private:
    Application::Interface app_;

    sf::Font uiFont_;

    sf::Text titleText_;

    Button toSandboxButton_;

    Button* clickedButton_ = nullptr;
};