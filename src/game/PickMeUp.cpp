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
    print::info("making self haste");
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

PickMeUp::OnPickUp StateGame::makeSelfSlow() {
    print::info("making self slow");
    constexpr auto VEL_CHANGE = -33;
    constexpr auto DEG_CHANGE = 5;
    return [this] (auto name) {
        print::info("{} got self slow", name);
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

PickMeUp::OnPickUp StateGame::makeOpponentHaste() {
    print::info("making opponent haste");
    constexpr auto VEL_CHANGE = 100;
    constexpr auto DEG_CHANGE = -15;
    return [this] (auto name) {
        print::info("{} got opponent haste", name);
        auto pickedBy = getPlayer(name);
        for (auto player = players_.begin(); player != players_.end(); ++player) {
            if (player == pickedBy) continue;
            player->changeVelocity(VEL_CHANGE);
            player->changeTurn(DEG_CHANGE);
            player->addTimedEffect(sf::milliseconds(2000), [this, name=player->name()] () {
                auto player = getPlayer(name);
                player->changeVelocity(-VEL_CHANGE);
                player->changeTurn(-DEG_CHANGE);
            });
        }
    };
}

PickMeUp::OnPickUp StateGame::makeOpponentSlow() {
    print::info("making opponent slow");
    constexpr auto VEL_CHANGE = -33;
    constexpr auto DEG_CHANGE = 5;
    return [this] (auto name) {
        print::info("{} got opponent slow", name);
        auto pickedBy = getPlayer(name);
        for (auto player = players_.begin(); player != players_.end(); ++player) {
            if (player == pickedBy) continue;
            player->changeVelocity(VEL_CHANGE);
            player->changeTurn(DEG_CHANGE);
            player->addTimedEffect(sf::milliseconds(2000), [this, name=player->name()] () {
                auto player = getPlayer(name);
                player->changeVelocity(-VEL_CHANGE);
                player->changeTurn(-DEG_CHANGE);
            });
        }
    };
}
