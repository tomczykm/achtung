#pragma once

#include <memory>
#include <functional>

#include <SDL.h>

#include "CMakeDefine.hpp"
#include "SettingsHandler.hpp"
#include "IGameState.hpp"

class Application {
public:
    ~Application();

    bool init();
    int run();

    template <class State, typename... Ts>
    void enterState(Ts... args) {
        changeState_ = [this, args...] () {
            gameState_ = std::make_unique<State>(*this, args...);
        };
    }

    void quitGame() {
        quit_ = true;
    }

    SettingsHandler& settings() { return settings_; }
    SDL_Renderer* renderer() { return renderer_; }
    SDL_Event& events() { return events_; }
private:
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Event events_;

    SettingsHandler settings_;

    std::unique_ptr<IGameState> gameState_;
    std::function<void()> changeState_;
    bool quit_ = false;
};
