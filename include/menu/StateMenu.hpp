#pragma once

#include "IGameState.hpp"

#include <memory>
#include <map>
#include <cstdint>
#include <optional>
#include <TGUI/TGUI.hpp>

#include "Application.hpp"
#include "game/Player.hpp"

class StateMenu : public IGameState {
public:
    explicit StateMenu(const Application::Interface&);

    void input(const sf::Event&) override;
    void logic() override {}
    void render() override {}
private:
    using PlayerId = std::uint32_t;

    void addPlayer();
    std::vector<PlayerInfo> preparePlayerInfos();
    void loadGui();
    bool canStartGame();
    std::size_t getCurrentNumPlayers();
    void removePlayer(PlayerId, tgui::Panel::Ptr);

    void enterSetKeysMode(PlayerId);
    void setKey(PlayerId, sf::Keyboard::Key);
    void updateKeysLabel(PlayerId);

    void recalculatePlayerListPositions();

    Application::Interface app_;
    std::map<PlayerId, PlayerInfo> playerInfos_;

    std::optional<PlayerId> setKeysMode_;
};