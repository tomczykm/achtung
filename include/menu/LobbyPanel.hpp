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

    void loadGui() override;
    void input(const sf::Event&) override;

private:
    using PlayerId = std::uint32_t;

    void addPlayer();
    std::vector<PlayerInfo> preparePlayerInfos();
    bool canStartGame();
    std::size_t getCurrentNumPlayers();
    void removePlayer(PlayerId, tgui::Panel::Ptr);

    void enterSetKeysMode(PlayerId);
    void setKey(PlayerId, sf::Keyboard::Key);
    void updateKeysLabel(PlayerId);

    void recalculatePlayerListPositions();

    void startGame();

    std::map<PlayerId, PlayerInfo> playerInfos_;
    std::optional<PlayerId> setKeysMode_;
};
