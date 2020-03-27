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
    explicit LobbyPanel(Application::Interface&, tgui::Panel::Ptr);

    void input(const sf::Event&) override;

private:
    void loadGui();
    void loadProfiles();

    void addProfileEntry(ProfileId);
    void addPlayerEntry(ProfileId);
    std::vector<PlayerInfo> preparePlayerInfos();
    bool canStartGame();
    std::size_t getCurrentNumPlayers();
    void removePlayerEntry(ProfileId, tgui::Panel::Ptr);

    void enterSetKeysMode(ProfileId);
    void setKey(ProfileId, sf::Keyboard::Key);
    void updateKeysLabel(ProfileId);

    void recalculateListPositions();

    void startGame();

    std::map<ProfileId, PlayerInfo> playerInfos_;
    std::optional<ProfileId> setKeysMode_;
};
