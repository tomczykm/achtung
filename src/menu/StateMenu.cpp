#include "menu/StateMenu.hpp"

StateMenu::StateMenu(const Application::Interface& app):
    app_(app)
{
    uiFont_.loadFromFile("ui.ttf");

    titleText_.setFont(uiFont_);
    titleText_.setString("freeachtung");
    titleText_.setCharacterSize(24);
    titleText_.setFillColor(sf::Color::Yellow);
    titleText_.setStyle(sf::Text::Italic);
    titleText_.setPosition(30, 30);

    print::info("StateMenu ready");
}

void StateMenu::input(const sf::Event&) {

}

void StateMenu::logic() {

}

void StateMenu::render() {
    app_.window.draw(titleText_);
}