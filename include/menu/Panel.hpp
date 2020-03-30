#pragma once

#include <string_view>

#include <SFML/Window/Event.hpp>
#include <TGUI/TGUI.hpp>

#include "app/Application.hpp"

enum PanelType {
    Lobby = 0,
    Settings,
    Profile,

    Count
};

class StateMenu;

class Panel {
public:
    Panel(Application::Interface& i, StateMenu& gs, tgui::Panel::Ptr p):
        app_{i},
        gs_{gs},
        panel_{p}
    {}

    virtual ~Panel() = default;

    virtual void input(const sf::Event&) = 0;

    void activate() {
        panel_->setEnabled(true);
        panel_->setVisible(true);
        onActivate();
    };

    void deactivate() {
        panel_->setEnabled(false);
        panel_->setVisible(false);
    };

protected:
    virtual void onActivate() {}

    Application::Interface& app_;
    StateMenu& gs_;

    tgui::Panel::Ptr panel_;
};
