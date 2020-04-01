#include "engine/EngineTestable.hpp"

namespace {

constexpr auto playerToGameAreaSizeRatio = 253.334f;
constexpr auto playerSpeedToGameAreaSizeRatio = 7.6f;

}

EngineTestable::EngineTestable(AssetManager& a, int tickrate, int playAreaCorner, int playAreaSide):
    Engine{a, {}, tickrate, playAreaCorner, playAreaSide}
{
    prepareTestPlayer();
}

void EngineTestable::prepareTestPlayer() {
    const auto radius = playAreaSideLength_ / playerToGameAreaSizeRatio;
    const auto velocity = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;

    auto timer = timerService_.makeTimer(sf::milliseconds(200));
    timer->pause();

    players_.emplace(0, std::make_unique<PlayerTestable>(radius, velocity, timer));
}

void EngineTestable::createPickMeUp(PickUpType type, int x, int y) {
    auto [onPickMeUp, texture] = makePickMeUpEffectAndTexture(type);
    pickmeups_.emplace_back(
        x, y,
        pickMeUpRadius_,
        assets_.getTexture(texture),
        onPickMeUp
    );
}
