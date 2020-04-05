#include "testable/EngineTestable.hpp"

EngineTestable::EngineTestable(IAssetManager& a, const InputSequence& in, int tickrate, float playAreaCorner, float playAreaSide):
    Engine{a, {}, tickrate, playAreaCorner, playAreaSide},
    inputs_{in}
{
    addPlayer(PlayerInfo{"test", sf::Keyboard::Q, sf::Keyboard::W, sf::Color::Red});
}

PlayerTestable& EngineTestable::addPlayer(const PlayerInfo& info) {
    auto timer = timerService_.makeTimer(sf::milliseconds(200));
    timer->pause();

    const auto newId = players_.size() == 0 ? 0 : (players_.rbegin()->first)+1;

    const auto [it, emplaced] = players_.emplace(newId, std::make_unique<PlayerTestable>(inputs_, info, timer));
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
