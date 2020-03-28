#pragma once

#include <optional>

#include "app/ProfileManager.hpp"
#include "menu/Panel.hpp"

class ProfilePanel : public Panel {
public:
    ProfilePanel(Application::Interface&, StateMenu&, tgui::Panel::Ptr);

    void input(const sf::Event&) override;

    void loadProfile(std::optional<ProfileId>);
private:
    void loadGui();
};
