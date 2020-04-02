#pragma once

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>

#include <SFML/Graphics.hpp>

enum class TextureType {
    Null = 0,
    SelfHaste,
    OpponentHaste,
    SelfSlow,
    OpponentSlow,
    SelfRightAngle,
    OpponentRightAngle,
    ClearTrails,
    ControlSwap,
    MassPowerups,
    RandomPickMeUp,
    Shrink,
    Enlarge
};

class IAssetManager {
public:
    using TextureSet = const std::set<TextureType>;

    virtual void loadTextures(TextureSet&) = 0;
    virtual void releaseTextures(TextureSet&) = 0;

    virtual const sf::Texture& getTexture(TextureType) = 0;
};

class AssetManager : public IAssetManager {
public: 
    static std::string getActualFileName(std::string_view resourceName);
    static std::stringstream openResource(std::string_view resourceName);

    void loadTextures(TextureSet&);
    void releaseTextures(TextureSet&);

    const sf::Texture& getTexture(TextureType);

private:
    std::map<TextureType, sf::Texture> textures_;

};
