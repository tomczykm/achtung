#pragma once

#include <string_view>

#include <SFML/Window/Event.hpp>
#include <TGUI/TGUI.hpp>

#include "app/Application.hpp"

class Panel {
public:
    Panel(Application::Interface& i, tgui::Panel::Ptr p):
        app_{i},
        panel_{p}
    {}

    virtual ~Panel() = default;

    virtual void loadGui() = 0;
    virtual void input(const sf::Event&) = 0;

    void activate() {
        panel_->setEnabled(true);
        panel_->setVisible(true);
    };

    void deactivate() {
        panel_->setEnabled(false);
        panel_->setVisible(false);
    };

protected:
    Application::Interface& app_;

    tgui::Panel::Ptr panel_;
};
