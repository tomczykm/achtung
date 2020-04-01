#pragma once

#include <gmock/gmock.h>

#include "app/AssetManager.hpp"

namespace mock {

class AssetManager : public IAssetManager {
public:
    MOCK_METHOD(void, loadTextures, (TextureSet&), (override));
    MOCK_METHOD(void, releaseTextures, (TextureSet&), (override));
    MOCK_METHOD(const sf::Texture&, getTexture, (TextureType), (override));
};

}  // namespace mock
