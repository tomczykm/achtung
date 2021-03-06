#include "app/Application.hpp"

#include "menu/StateMenu.hpp"
#include "CMakeDefine.hpp"

Application::Application():
    gui_{window_},
    profiles_{database_}
{
    const auto tickrate = config_.get<int>(Setting::TickRate);
    stepTime_ = sf::milliseconds(1000/tickrate);
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;

    const auto fullscreenFlag = config_.get<bool>(Setting::Fullscreen) ? sf::Style::Fullscreen : 0;

    print::info("before create");
    window_.create(sf::VideoMode{config_.get<uint32_t>(Setting::ResWidth), config_.get<uint32_t>(Setting::ResHeight)},
        projectName.data(), sf::Style::Titlebar | sf::Style::Close | fullscreenFlag, windowSettings);
    window_.setVerticalSyncEnabled(true);
    print::info("after create");

    print::info("Initialization complete");
}

int Application::run() {
    auto accumulator = sf::milliseconds(0);

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

        accumulator += deltaTimeClock_.getElapsedTime();
        while (accumulator > stepTime_) {
            gameState_->tick(stepTime_.asMilliseconds() / 1000.f);
            accumulator -= stepTime_;
        }
        deltaTimeClock_.restart();

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
