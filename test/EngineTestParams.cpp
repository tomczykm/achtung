#include "EngineTestParams.hpp"

namespace {

const auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
    player.setDead(false);
    player.setPosition(150, 950);
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
            for (int y = 900; y < 1000; y+=2)
                t.emplace_back(200, y, 0, player.getRadius(), sf::Color::Red);
        };

        engine.accessState(resetState);
        engine.enablePowerups(false);
    }, 140),

    // 1 - ShouldntDieOnEndOfHaste
    // tests for a bug where end of haste
    // does weird thing with player position
    // and creates a long straight trails
    // sometimes catapults player outside the bounds
    std::make_pair([] (auto& engine, auto& inputs) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfHaste, 200, 950);
        inputs.addKeyHold(sf::Keyboard::Q, 30, 120);
        inputs.addKeyHold(sf::Keyboard::W, 140, 280);
        inputs.addKeyHold(sf::Keyboard::Q, 310, 500);
    }, 700),

    // 2 - PickupsShouldNotCancelEachOtherOut
    std::make_pair([] (auto& engine, auto&) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfRightAngle, 250, 950);
        engine.createPickMeUp(PickUpType::SelfRightAngle, 450, 950);
    }, 450),

    // 3 - CheckSwapControls
    std::make_pair([] (auto& engine, auto& inputs) {
        auto resetState = [] (auto& player, auto& pickups, auto& t) {
            ::resetState(player, pickups, t);
            player.setPosition(150, 250);
        };
        engine.enablePowerups(false);
        engine.accessState(resetState);
        auto& p2 = engine.addPlayer(PlayerInfo{"be", sf::Keyboard::A, sf::Keyboard::S, sf::Color::Green});
        p2.setPosition(150, 100);
        p2.setDirection(270);
        engine.createPickMeUp(PickUpType::ControlSwap, 250, 100);
        inputs.addKeyHold(sf::Keyboard::Q, 450, 550);
    }, 1200),

    // 4 - CheckHasteAndRightAngledInteraction
    // at the moment of writing this test,
    // picking up right angle cancels haste
    std::make_pair([] (auto& engine, auto&) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfHaste, 250, 950);
        engine.createPickMeUp(PickUpType::SelfRightAngle, 450, 950);
    }, 400),

    // 5 - CheckOpponentSlow
    std::make_pair([] (auto& engine, auto& inputs) {
        auto resetState = [] (auto& player, auto& pickups, auto& t) {
            ::resetState(player, pickups, t);
            player.setPosition(150, 250);
        };
        engine.enablePowerups(false);
        engine.accessState(resetState);
        auto& p2 = engine.addPlayer(PlayerInfo{"be", sf::Keyboard::A, sf::Keyboard::S, sf::Color::Green});
        p2.setPosition(150, 100);
        p2.setDirection(270);
        engine.createPickMeUp(PickUpType::OpponentSlow, 250, 100);
        inputs.addKeyHold(sf::Keyboard::W, 450, 550);
    }, 1200),

    // 6 - CheckMultipleHaste
    std::make_pair([] (auto& engine, auto& inputs) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::SelfHaste, 250, 950);
        engine.createPickMeUp(PickUpType::SelfHaste, 450, 950);
        inputs.addKeyHold(sf::Keyboard::Q, 250, 350);
        inputs.addKeyHold(sf::Keyboard::Q, 400, 500);
    }, 500),

    // 7 - CheckEnlargeAndShrink
    std::make_pair([] (auto& engine, auto& inputs) {
        auto resetState = [] (auto& player, auto& pickups, auto& t) {
            ::resetState(player, pickups, t);
            player.setPosition(150, 250);
        };
        engine.enablePowerups(false);
        engine.accessState(resetState);
        auto& p2 = engine.addPlayer(PlayerInfo{"be", sf::Keyboard::A, sf::Keyboard::S, sf::Color::Green});
        p2.setPosition(150, 100);
        p2.setDirection(270);
        engine.createPickMeUp(PickUpType::Shrink, 250, 250);
        engine.createPickMeUp(PickUpType::Shrink, 300, 250);
        engine.createPickMeUp(PickUpType::Enlarge, 350, 250);
        engine.createPickMeUp(PickUpType::Enlarge, 400, 250);
        inputs.addKeyHold(sf::Keyboard::W, 450, 550);
        inputs.addKeyHold(sf::Keyboard::S, 550, 650);
    }, 1200),

    // 8 - CheckWarping
    std::make_pair([] (auto& engine, auto& inputs) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::MapWarp, 350, 950);
        inputs.addKeyHold(sf::Keyboard::W, 230, 280);
    }, 600),

    // 9 - CheckNoTrails
    std::make_pair([] (auto& engine, auto& inputs) {
        engine.enablePowerups(false);
        engine.accessState(resetState);
        engine.createPickMeUp(PickUpType::NoTrails, 200, 950);
        inputs.addKeyHold(sf::Keyboard::W, 30, 120);
    }, 600),
}};
