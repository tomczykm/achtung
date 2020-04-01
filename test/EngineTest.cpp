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

const auto resetState = [] (PlayerTestable& player, Engine::Pickmeups&, Engine::Trails& t) {
    player.newRoundSetup(100, 900, t);
    player.setDirection(270);
    t.clear();
};

const auto printPlayer = [] (PlayerTestable& player, Engine::Pickmeups&, Engine::Trails&) {
    auto pos = player.getPosition();
    print::info("Position: {}x{}, dead={}", pos.x, pos.y, player.isDead());
};

const auto assertPlayerDead = [] (auto& player, auto&, auto&) {
    EXPECT_TRUE(player.isDead());
};

}  // namespace

using namespace ::testing;

struct EngineTest : Test {

    EngineTest():
        engine_{
            assetManager_,
            tickrate,
            0,
            0.9 * 1080
        }
    {
        print::detail::logFile.reset(std::fopen(fmt::format("{}Test.log", UnitTest::GetInstance()->current_test_info()->name()).c_str(), "w"));

        ON_CALL(assetManager_, getTexture(_))
            .WillByDefault(ReturnRef(defaultTex));
    }

    NiceMock<mock::AssetManager> assetManager_;
    EngineTestable engine_;

    sf::Texture defaultTex;
};

TEST_F(EngineTest, DieOnCollisionWithTrail) {
    auto resetState = [] (PlayerTestable& player, Engine::Pickmeups&, Engine::Trails& t) {
        player.newRoundSetup(100, 900, t);
        player.setDirection(270);
        t.clear();
        for (int y = 850; y < 950; y+=2)
            t.emplace_back(150, y, 0, player.getRadius(), sf::Color::Red);
    };

    engine_.accessState(resetState);
    for (auto tick = 0u; tick < 140; ++tick) {
        engine_.step(stepTime);
    }
    engine_.accessState(assertPlayerDead);
}
