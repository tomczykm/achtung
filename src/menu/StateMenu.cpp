#include "menu/StateMenu.hpp"
#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

StateMenu::StateMenu(const Application::Interface& app):
    app_(app),
    buttons_{
        Button{{500, 300}, "Go to sandbox", uiFont_, [this] () {
            app_.enterState<StateSandbox>();
        }},
        Button{{300, 300}, "Start game", uiFont_, [this] () {
            app_.enterState<StateGame>();
        }}
    }
{
    uiFont_.loadFromFile("ui.ttf");

    titleText_.setFont(uiFont_);
    titleText_.setString("freeachtung");
    titleText_.setCharacterSize(32);
    titleText_.setFillColor(sf::Color::Yellow);
    titleText_.setStyle(sf::Text::Italic);
    titleText_.setPosition(30, 30);

    print::info("StateMenu ready");
}

void StateMenu::input(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonReleased
        && event.mouseButton.button == sf::Mouse::Left)
    {
        for (const auto& button: buttons_) {
            if (button.getClickableArea().contains(event.mouseButton.x, event.mouseButton.y)) {
                clickedButton_ = &button;
            }
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.quit();
        }
    }
}

void StateMenu::logic() {
    if (clickedButton_ != buttons_.end()) {
        clickedButton_->action();
        clickedButton_ = buttons_.end();
    }
}

void StateMenu::render() {
    app_.window.draw(titleText_);

    for (const auto& button: buttons_) {
        button.draw(app_.window);
    }
}