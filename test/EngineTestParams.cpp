#include "EngineTestParams.hpp"

namespace {

const auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
    player.setDead(false);
    player.setPosition(100, 900);
    player.setDirection(270);
    t.clear();
    pickups.clear();
};

}

const std::array<TestPreviewParam, totalTestcases> setupAndTicks = {{
    // 0 - ShouldDieOnCollisionWithTrail
    std::make_pair([] (auto& engine, auto&) {
        auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
            ::resetState(player, pickups, t);
            for (int y = 850; y < 950; y+=2)
                t.emplace_back(150, y, 0, player.getRadius(), sf::Color::Red);
        };

        engine.accessState(resetState);
    }, 140),

    // 1 - ShouldntDieOnEndOfHaste
    // tests for a bug where end of haste
    // does weird thing with player position
    // and creates a long straight trails
    // sometimes catapults player outside the bounds
    std::make_pair([] (auto& engine, auto& inputs) {
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfHaste, 150, 900);
        inputs.addKeyHold(sf::Keyboard::Q, 30, 120);
        inputs.addKeyHold(sf::Keyboard::W, 140, 280);
        inputs.addKeyHold(sf::Keyboard::Q, 310, 500);
    }, 700),

    // 2 - PickupsShouldNotCancelEachOtherOut
    std::make_pair([] (auto& engine, auto&) {
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfRightAngle, 200, 900);
        engine.createPickMeUp(PickUpType::SelfRightAngle, 400, 900);
    }, 450)
}};
