#pragma once

#include "menu/Panel.hpp"

#include "app/Application.hpp"

class SettingsPanel : public Panel {
public:
    SettingsPanel(Application::Interface&, StateMenu&, tgui::Panel::Ptr);

    void input(const sf::Event&) override;

private:
    void loadGui();
};
