#pragma once

#include "app/IGameState.hpp"

#include <memory>
#include <array>
#include <TGUI/TGUI.hpp>

#include "menu/Panel.hpp"

#include "app/Application.hpp"
#include "app/Utils.hpp"

class StateMenu : public IGameState {
public:
    explicit StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void tick(double) override {}
    void render() override {}

    void setActivePanel(PanelType);

    template <typename T>
    [[nodiscard]] T& setActivePanel(PanelType t) {
        setActivePanel(t);
        return dynamic_cast<T&>(**activePanel_);
    }

private:
    using Panels = std::array<std::unique_ptr<Panel>, PanelType::Count>;

    void loadGui();

    template <typename T> void makePanel(PanelType p);
    tgui::Panel::Ptr makeGuiPanel();

    Application::Interface app_;

    Panels panels_;
    Panels::iterator activePanel_;
};
