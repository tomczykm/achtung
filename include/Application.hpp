#pragma once

#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "Log.hpp"
#include "SettingsManager.hpp"
#include "IGameState.hpp"
#include "Utils.hpp"

class Application {
public:
    Application();

    int run();

    class Interface;

private:
    SettingsManager settings_;
    sf::RenderWindow window_;
    tgui::Gui gui_;

    std::unique_ptr<IGameState> gameState_;
    std::function<void()> changeState_;
    bool quit_ = false;
};

class Application::Interface
{
public:
    SettingsManager& settings;
    sf::RenderWindow& window;
    tgui::Gui& gui;

    void quit() {
        print::info("quit");
        app_.quit_ = true;
    }

    template <class State, typename... Ts>
    void enterState(Ts... args) {
        print::info("Changing state to {}", stripLeadingDigits(typeid(State).name()));
        app_.changeState_ = [this, args...] () {
            app_.gameState_ = std::make_unique<State>(*this, args...);
        };
    }

private:
    friend class Application;
    explicit Interface(Application& app):
        settings{app.settings_},
        window{app.window_},
        gui{app.gui_},
        app_{app}
    {}

    Application& app_;
};