#include "menu/StateMenu.hpp"
#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

StateMenu::StateMenu(const Application::Interface& app):
    app_{app}
{
    loadGui();
    print::info("StateMenu ready");
}

void StateMenu::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.quit();
        }
    }
}

void StateMenu::addPlayer() {
    print::info(__func__);
}

void StateMenu::loadGui() {
    app_.gui.loadWidgetsFromFile("../menu.gui");

    app_.gui.get("AddPlayer")->connect("pressed", [this] () {
        addPlayer();
    });

    app_.gui.get("StartGame")->connect("pressed", [this] () {
        app_.enterState<StateGame>();
    });

    app_.gui.get("QuitGame")->connect("pressed", [this] () {
        app_.quit();
    });
}