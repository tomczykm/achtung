#pragma once

#include "menu/Panel.hpp"

#include "app/Application.hpp"

class SettingsPanel : public Panel {
public:
    explicit SettingsPanel(Application::Interface&, tgui::Panel::Ptr);

    void loadGui() override;
    void input(const sf::Event&) override;

private:

};
