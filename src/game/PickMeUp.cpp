#include "game/PickMeUp.hpp"

#include "game/StateGame.hpp"

namespace
{
constexpr uint32_t RADIUS = 14;
}

PickMeUp::PickMeUp(uint32_t xPos, uint32_t yPos, OnPickUp f):
    shape_{RADIUS},
    onPickUp_{f}
{
    shape_.setOrigin(RADIUS, RADIUS);
    shape_.setPosition(xPos, yPos);
    shape_.setFillColor(sf::Color::Blue);
}

PickMeUp::OnPickUp StateGame::makeSelfHaste() {
    constexpr auto VEL_CHANGE = 100;
    constexpr auto DEG_CHANGE = -30;
    return [this] (auto name) {
        print::info("{} got self haste", name);
        auto player = getPlayer(name);
        player->changeVelocity(VEL_CHANGE);
        player->changeTurn(DEG_CHANGE);
        player->addTimedEffect(sf::milliseconds(2500), [this, name] () {
            auto player = getPlayer(name);
            player->changeVelocity(-VEL_CHANGE);
            player->changeTurn(-DEG_CHANGE);
        });
    };
}
