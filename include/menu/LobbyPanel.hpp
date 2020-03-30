#pragma once

#include "menu/Panel.hpp"

#include <map>
#include <optional>
#include <cstdint>
#include <vector>

#include "app/Application.hpp"
#include "game/Player.hpp"

class LobbyPanel : public Panel {
public:
    LobbyPanel(Application::Interface&, StateMenu&, tgui::Panel::Ptr);

    void input(const sf::Event&) override;

private:
    void onActivate() override;
    void updateProfileEntries();

    void loadGui();

    void addProfileEntry(ProfileId);
    void addPlayerEntry(ProfileId);
    bool canStartGame();
    std::size_t getCurrentNumPlayers();
    void removePlayerEntry(ProfileId, tgui::Panel::Ptr);

    void enterSetKeysMode(ProfileId);
    void setKey(ProfileId, sf::Keyboard::Key);
    void updateKeysLabel(ProfileId);

    void recalculateListPositions();

    void startGame();

    PlayerInfos playerInfos_;
    std::optional<ProfileId> setKeysMode_;
};
