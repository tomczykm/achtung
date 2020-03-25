#pragma once

#include "app/IGameState.hpp"

#include <memory>
#include <array>
#include <TGUI/TGUI.hpp>

#include "menu/Panel.hpp"

#include "app/Application.hpp"

class StateMenu : public IGameState {
public:
    explicit StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void tick(double) override {}
    void render() override {}

    void setActivePanel(int);

    constexpr static auto totalPanels = 2u;

private:
    using Panels = std::array<std::unique_ptr<Panel>, totalPanels>;

    void loadGui();
    tgui::Panel::Ptr makePanel(tgui::PanelRenderer*);

    Application::Interface app_;

    Panels panels_;
    Panels::iterator activePanel_;

};
