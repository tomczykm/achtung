#include "testable/EngineTestable.hpp"

namespace {

constexpr auto playerToGameAreaSizeRatio = 253.334f;
constexpr auto playerSpeedToGameAreaSizeRatio = 7.6f;

}

EngineTestable::EngineTestable(IAssetManager& a, const InputSequence& in, int tickrate, int playAreaCorner, int playAreaSide):
    Engine{a, {}, tickrate, playAreaCorner, playAreaSide},
    inputs_{in}
{
    addPlayer(PlayerInfo{"test", sf::Keyboard::Q, sf::Keyboard::W, sf::Color::White});
}

PlayerTestable& EngineTestable::addPlayer(const PlayerInfo& info) {
    const auto radius = playAreaSideLength_ / playerToGameAreaSizeRatio;
    const auto velocity = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;

    auto timer = timerService_.makeTimer(sf::milliseconds(200));
    timer->pause();

    const auto newId = players_.size() == 0 ? 0 : (players_.rbegin()->first)+1;

    const auto [it, emplaced] = players_.emplace(newId, std::make_unique<PlayerTestable>(inputs_, info, radius, velocity, timer));
    return dynamic_cast<PlayerTestable&>(*(it->second));
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