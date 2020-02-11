#include "menu/StateMenu.hpp"
#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

StateMenu::StateMenu(const Application::Interface& app):
    app_{app},
    titleLabel_{tgui::Label::create()},
    startGameButton_{tgui::Button::create()},
    quitButton_{tgui::Button::create()}
{
    titleLabel_->setText("freeachtung");
    titleLabel_->setTextSize(32);
    titleLabel_->setPosition({30, 30});
    titleLabel_->getRenderer()->setTextColor(sf::Color::Yellow);

    startGameButton_->setText("Start");
    startGameButton_->setSize(140, 60);
    startGameButton_->setTextSize(18);
    startGameButton_->setPosition({100, 700});
    startGameButton_->connect("pressed", [this] () {
        app_.enterState<StateGame>();
    });

    quitButton_->setText("Quit");
    quitButton_->setSize(140, 60);
    quitButton_->setTextSize(18);
    quitButton_->setPosition({400, 700});
    quitButton_->connect("pressed", [this] () {
        app_.quit();
    });

    app_.gui.add(titleLabel_);
    app_.gui.add(startGameButton_);
    app_.gui.add(quitButton_);

    print::info("StateMenu ready");
}

void StateMenu::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.quit();
        }
    }
}

void StateMenu::logic() {

}

void StateMenu::render() {

}

void StateMenu::addPlayer() {

}