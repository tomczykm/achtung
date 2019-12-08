#include <cstdlib>
#include <ctime>

#include "Application.hpp"

#include "menu/StateMenu.hpp"

Application::Application():
    settings_("settings.dat"),
    gameState_(std::make_unique<StateMenu>(Interface{*this}))
{
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;

    window_.create(sf::VideoMode(settings_.getResW(), settings_.getResH()),
        PROJECT_NAME, sf::Style::Titlebar | sf::Style::Close, windowSettings);
    window_.setVerticalSyncEnabled(true);
    window_.setPosition({100, 100});

    print::info("Initialization complete");
}

int Application::run() {
    while (!quit_) {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                quit_ = true;
            }

            gameState_->input(event);
        }

        gameState_->logic();

        if (changeState_) {
            changeState_();
            changeState_ = nullptr;
        }

        window_.clear(sf::Color::Black);
        gameState_->render();
        window_.display();
    }
    return 0;
}
