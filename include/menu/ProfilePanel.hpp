#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "app/ProfileManager.hpp"
#include "menu/Panel.hpp"

class ProfilePanel : public Panel {
public:
    ProfilePanel(Application::Interface&, StateMenu&, tgui::Panel::Ptr);

    void input(const sf::Event&) override;

    void loadProfile(std::optional<ProfileId>);
private:
    void loadGui();

    void saveProfile();
    void deleteProfile();

    sf::Color decodeColor(const std::string&);
    std::string encodeColor(sf::Color);

    std::optional<ProfileId> loadedProfile_;
};
