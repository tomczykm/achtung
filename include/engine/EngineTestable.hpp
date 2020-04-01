#pragma once

#include "engine/Engine.hpp"

class EngineTestable : public Engine {
public:
    EngineTestable(IAssetManager&, int tickrate, int playAreaCorner, int playAreaSide);

    void enablePowerups(bool enabled) {
        pickmeupSpawnTimer_->pause(!enabled);
    }

    template <typename TransformFunc>
    void accessState(TransformFunc&& f) {
        f(dynamic_cast<PlayerTestable&>(*players_.begin()->second), pickmeups_, trails_);
    }

    void createPickMeUp(PickUpType, int x, int y);

private:
    void prepareTestPlayer();
};
