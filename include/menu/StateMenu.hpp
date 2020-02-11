#pragma once

#include "IGameState.hpp"

#include <optional>
#include <SFML/Graphics.hpp>

#include "Application.hpp"

class StateMenu : public IGameState {
public:
    explicit StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override;
    void render() override;
private:
    void addPlayer();

    Application::Interface app_;

    sf::Font uiFont_;
    sf::Text titleText_;
};