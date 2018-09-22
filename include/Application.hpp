#pragma once

#include <string>
#include <memory>
#include <functional>

#include <SDL.h>

#include "CMakeDefine.hpp"
#include "SettingsHandler.hpp"
#include "IGameState.hpp"

class Application {
public:
    Application();
    ~Application();

    bool init();
    int run();

    template<class State, typename... Ts>
    void enterState(Ts... args) {
        changeState_ = [this, args...] () {
            gameState_.reset(new State(*this, args...));
        };
    }

    void quitGame() {
        quit_ = true;
    }

    SettingsHandler& settings() { return settings_; }
    SDL_Renderer* renderer() { return renderer_; }
    SDL_Event& events() { return events_; }
private:
    SDL_Window *window_;
    SDL_Event events_;
    SDL_Renderer *renderer_;

    SettingsHandler settings_;

    std::unique_ptr<IGameState> gameState_;
    std::function<void()> changeState_;
    bool quit_;

    const std::string projectName_;
};
