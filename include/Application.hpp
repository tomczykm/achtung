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

    class Interface;

private:
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Event events_;

    SettingsHandler settings_;

    std::unique_ptr<IGameState> gameState_;
    std::function<void()> changeState_;
    bool quit_ = false;
};

class Application::Interface
{
public:
    SettingsHandler& settings;
    SDL_Renderer& renderer;
    SDL_Event& events;

    void quit() {
        app_.quit_ = true;
    }

    template <class State, typename... Ts>
    void enterState(Ts... args) {
        app_.changeState_ = [this, args...] () {
            app_.gameState_ = std::make_unique<State>(*this, args...);
        };
    }

private:
    friend class Application;
    explicit Interface(Application& app):
        settings(app.settings_),
        renderer(*app.renderer_),
        events(app.events_),
        app_(app)
    {}

    Application& app_;
};