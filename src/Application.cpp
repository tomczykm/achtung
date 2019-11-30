#include <cstdlib>
#include <ctime>

#include "Application.hpp"
#include "Log.hpp"

#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

Application::Application():
    settings_("settings.dat"),
    window_(sf::VideoMode(settings_.getResW(), settings_.getResH()),
        PROJECT_NAME, sf::Style::Titlebar | sf::Style::Close),
    gameState_(std::make_unique<StateSandbox>(Interface{*this}))
{
    std::srand(std::time(nullptr));
    window_.setVerticalSyncEnabled(true);
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
        }

        gameState_->input();

        gameState_->logic();
        if (changeState_) {
            changeState_();
            changeState_ = nullptr;
        }

        // SDL_RenderClear(renderer_);
        gameState_->render();
        // SDL_RenderPresent(renderer_);
    }
    return 0;
}
