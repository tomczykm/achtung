#pragma once

#include "IGameState.hpp"

#include <SFML/Graphics.hpp>

#include "Application.hpp"

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
};