#pragma once

#include "IGameState.hpp"

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
    void addPlayer();
    std::vector<PlayerInfo> preparePlayerInfos();
    void loadGui();
    std::size_t getCurrentNumPlayers();

    Application::Interface app_;
    std::uint32_t latestPlayerId_ = 0u;
};