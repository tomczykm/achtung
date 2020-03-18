#pragma once

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>

#include <SFML/Graphics.hpp>

class AssetManager {
public:
    enum class Texture {
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
        RandomPickMeUp
    };
    using TextureSet = const std::set<AssetManager::Texture>;

    static std::string getActualFileName(std::string_view resourceName);
    static std::stringstream openResource(std::string_view resourceName);

    void loadTextures(TextureSet&);
    void releaseTextures(TextureSet&);

    const sf::Texture& getTexture(Texture);

private:
    std::map<Texture, sf::Texture> textures_;

};
