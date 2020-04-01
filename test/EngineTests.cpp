#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/EngineTestable.hpp"

#include "mock/AssetManager.hpp"

namespace print::detail {
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

namespace {

constexpr static auto tickrate = 140;
const auto stepTime = sf::milliseconds(1000/tickrate).asMilliseconds() / 1000.f;

constexpr auto boxSize = 0.9 * 1080;

const auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
    player.setDead(false);
    player.setPosition(100, 900);
    player.setDirection(270);
    t.clear();
    pickups.clear();
};

const auto assertPlayerDead = [] (auto& player, auto&, auto&) {
    EXPECT_TRUE(player.isDead());
};

const auto assertPlayerNotDead = [] (auto& player, auto&, auto&) {
    EXPECT_FALSE(player.isDead());
};

const auto assertPlayerInBoxBounds = [] (auto& player, auto&, auto&) {
    auto [x, y] = player.getPosition();
    EXPECT_TRUE(x < boxSize);
    EXPECT_TRUE(y < boxSize);
};

}  // namespace

using namespace ::testing;

struct EngineTests : Test {

    EngineTests():
        engine_{
            assetManager_,
            inputs_,
            tickrate,
            0,
            boxSize
        }
    {
        print::detail::logFile.reset(std::fopen(fmt::format("{}Test.log", UnitTest::GetInstance()->current_test_info()->name()).c_str(), "w"));
        engine_.accessState(resetState);
        engine_.enablePowerups(false);

        ON_CALL(assetManager_, getTexture(_))
            .WillByDefault(ReturnRef(defaultTex_));
    }

    void stepEngine(uint32_t n) {
        for (auto tick = 0u; tick < n; ++tick) {
            sf::Event event;
            while (inputs_.pollEvent(event)) {
                engine_.input(event);
            }
            engine_.step(stepTime);
            inputs_.advance();
        }
    }

    NiceMock<mock::AssetManager> assetManager_;
    EngineTestable engine_;
    InputSequence inputs_;

    sf::Texture defaultTex_;
};

TEST_F(EngineTests, DieOnCollisionWithTrail) {
    auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
        ::resetState(player, pickups, t);
        for (int y = 850; y < 950; y+=2)
            t.emplace_back(150, y, 0, player.getRadius(), sf::Color::Red);
    };

    engine_.accessState(resetState);
    stepEngine(140);
    engine_.accessState(assertPlayerDead);
}

TEST_F(EngineTests, ShouldntDieOnEndOfHaste) {
    // tests for a bug where end of haste
    // does weird thing with player position
    // and creates a long straight trails
    // sometimes catapults player outside the bounds

    engine_.accessState(resetState);

    engine_.createPickMeUp(PickUpType::SelfHaste, 150, 900);
    inputs_.addKeyHold(sf::Keyboard::Q, 30, 120);
    inputs_.addKeyHold(sf::Keyboard::W, 140, 280);
    inputs_.addKeyHold(sf::Keyboard::Q, 310, 500);
    stepEngine(700);
    engine_.accessState(assertPlayerNotDead);
    engine_.accessState(assertPlayerInBoxBounds);
}
