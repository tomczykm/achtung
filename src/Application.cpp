#include "Application.hpp"

#include "menu/StateMenu.hpp"
#include "CMakeDefine.hpp"

Application::Application():
    settings_{"settings.dat"},
    gui_{window_},
    gameState_{std::make_unique<StateMenu>(Interface{*this})}
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
            gui_.handleEvent(event);
        }

        gameState_->logic();

        if (changeState_) {
            gui_.removeAllWidgets();
            changeState_();
            changeState_ = nullptr;
        }

        window_.clear(sf::Color::Black);
        gameState_->render();
        gui_.draw();
        window_.display();
    }
    return 0;
}
