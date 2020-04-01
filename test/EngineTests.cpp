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

struct EngineTests : TestWithParam<EngineTestParam> {

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

TEST_P(EngineTests, TestEngine) {
    const auto [setup, asserts, ticks] = GetParam();

    setup(engine_, inputs_);
    stepEngine(ticks);
    asserts(engine_);
}

INSTANTIATE_TEST_SUITE_P(Tests, EngineTests, Values(
    // 0 - ShouldDieOnCollisionWithTrail
    std::make_tuple(setupAndTicks[0].first, [] (auto& engine) {
        engine.accessState(assertIsPlayerDead(true));
    }, setupAndTicks[0].second),

    // 1 - ShouldntDieOnEndOfHaste
    std::make_tuple(setupAndTicks[1].first, [] (auto& engine) {
        engine.accessState(assertIsPlayerDead(false));
        engine.accessState(assertPlayerInBoxBounds);
    }, setupAndTicks[1].second),

    // 2 - PickupsShouldNotCancelEachOtherOut
    std::make_tuple(setupAndTicks[2].first, [] (auto& engine) {
        engine.accessState([] (auto& p, auto&, auto&) {
            EXPECT_EQ(p.getEffects().size(), 2);
        });
        engine.accessState([] (auto& p, auto&, auto&) {
            EXPECT_TRUE(p.isRightAngled());
        });
    }, setupAndTicks[2].second)
));
