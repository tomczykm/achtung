#pragma once

#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "CMakeDefine.hpp"
#include "Log.hpp"
#include "SettingsHandler.hpp"
#include "IGameState.hpp"

class Application {
public:
    Application();

    int run();

    class Interface;

private:
    SettingsHandler settings_;
    sf::RenderWindow window_;

    std::unique_ptr<IGameState> gameState_;
    std::function<void()> changeState_;
    bool quit_ = false;
};

class Application::Interface
{
public:
    SettingsHandler& settings;
    sf::RenderWindow& window;

    void quit() {
        app_.quit_ = true;
    }

    template <class State, typename... Ts>
    void enterState(Ts... args) {
        print::info("Changing state to {}", typeid(State).name());
        app_.changeState_ = [this, args...] () {
            app_.gameState_ = std::make_unique<State>(*this, args...);
        };
    }

private:
    friend class Application;
    explicit Interface(Application& app):
        settings(app.settings_),
        window(app.window_),
        app_(app)
    {}

    Application& app_;
};