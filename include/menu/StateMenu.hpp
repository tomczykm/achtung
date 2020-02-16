#pragma once

#include "IGameState.hpp"

#include <memory>
#include <map>
#include <cstdint>
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
    std::size_t getCurrentNumPlayers();
    void removePlayer(PlayerId, tgui::Panel::Ptr);
    void recalculatePlayerListPositions();

    Application::Interface app_;
    std::map<PlayerId, PlayerInfo> playerInfos_;
};