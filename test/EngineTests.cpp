#include <functional>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "EngineTestParams.hpp"
#include "testable/EngineTestable.hpp"
#include "mock/AssetManager.hpp"

namespace print::detail {
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

namespace {

constexpr static auto tickrate = 140;
const auto stepTime = sf::milliseconds(1000/tickrate).asMilliseconds() / 1000.f;

constexpr auto boxSize = 0.9 * 1080;

auto assertIsPlayerDead(bool dead) {
    return [dead] (auto& player, auto&, auto&) {
        EXPECT_EQ(player.isDead(), dead);
    };
}

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
            50,
            boxSize
        }
    {
        print::detail::logFile.reset(std::fopen(fmt::format("{}Test.log", UnitTest::GetInstance()->current_test_info()->name()).c_str(), "w"));

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

TEST_F(EngineTests, ShouldDieOnCollisionWithTrail) {
    const auto [setup, ticks] = setupAndTicks[0];

    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState(assertIsPlayerDead(true));
}

TEST_F(EngineTests, ShouldntDieOnEndOfHaste) {
    const auto [setup, ticks] = setupAndTicks[1];

    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState(assertIsPlayerDead(false));
    engine_.accessState(assertPlayerInBoxBounds);

    // check if speed was changed; y>=600 says it was not (too close to starting pos)
    engine_.accessState([] (auto& p, auto&, auto&) {
        auto [x, y] = p.getPosition();
        EXPECT_LT(y, 600);
    });
}

TEST_F(EngineTests, PickupsShouldNotCancelEachOtherOut) {
    const auto [setup, ticks] = setupAndTicks[2];

    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState([] (auto& p, auto&, auto&) {
        EXPECT_EQ(p.getTotalEffects(), 2);
        EXPECT_TRUE(p.isRightAngled());
    });
}

TEST_F(EngineTests, CheckSwapControls) {
    const auto [setup, ticks] = setupAndTicks[3];
    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState([] (auto& p, auto&, auto&) {
        auto [x, y] = p.getPosition();
        EXPECT_GT(y, 200);
        EXPECT_EQ(p.getTotalEffects(), 0);
    });
}

TEST_F(EngineTests, CheckHasteAndRightAngledInteraction) {
    const auto [setup, ticks] = setupAndTicks[4];
    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState([] (auto& p, auto&, auto&) {
        EXPECT_EQ(p.getTotalEffects(), 2);
        EXPECT_TRUE(p.isRightAngled());
        EXPECT_FALSE(p.isOnBaseVelocity());
    });
}

TEST_F(EngineTests, CheckOpponentSlow) {
    const auto [setup, ticks] = setupAndTicks[5];
    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState([] (auto& p, auto&, auto&) {
        EXPECT_EQ(p.getTotalEffects(), 0);
        EXPECT_TRUE(p.isOnBaseVelocity());
    });
}

TEST_F(EngineTests, CheckMultipleHaste) {
    const auto [setup, ticks] = setupAndTicks[6];
    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState([] (auto& p, auto&, auto&) {
        EXPECT_EQ(p.getTotalEffects(), 2);
        EXPECT_FALSE(p.isOnBaseVelocity());
    });
}

TEST_F(EngineTests, CheckWarping) {
    const auto [setup, ticks] = setupAndTicks[8];
    setup(engine_, inputs_);
    stepEngine(ticks);
    engine_.accessState(assertIsPlayerDead(false));
}
