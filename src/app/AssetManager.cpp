#include "app/AssetManager.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

#include "app/Log.hpp"

namespace {

const std::map<TextureType, std::string_view> texToFilename = {
    {TextureType::SelfHaste, "sprites/selfHaste.png"},
    {TextureType::OpponentHaste, "sprites/oppHaste.png"},
    {TextureType::SelfSlow, "sprites/selfSlow.png"},
    {TextureType::OpponentSlow, "sprites/oppSlow.png"},
    {TextureType::SelfRightAngle, "sprites/selfSquare.png"},
    {TextureType::OpponentRightAngle, "sprites/oppSquare.png"},
    {TextureType::ClearTrails, "sprites/clear.png"},
    {TextureType::ControlSwap, "sprites/swap.png"},
    {TextureType::MassPowerups, "sprites/massPowerups.png"},
    {TextureType::RandomPickMeUp, "sprites/random.png"},
    {TextureType::Shrink, "sprites/shrink.png"},
    {TextureType::Enlarge, "sprites/enlarge.png"},
    {TextureType::SelfWarp, "sprites/selfWarp.png"},
    {TextureType::MapWarp, "sprites/mapWarp.png"}
};

}  // namespace

std::string AssetManager::getActualFileName(std::string_view resourceName) {
    print::info("{}: {}", __func__, resourceName);
    const std::array<std::string, 4> fileNames = {
        fmt::format("res/{}", resourceName),
        resourceName.data(),
        fmt::format("../res/{}", resourceName),  // for running from build directory
        fmt::format("../{}", resourceName)
    };

    for (const auto& name: fileNames) {
        if (std::filesystem::exists(name)) {
            print::info("{}: success! the filename is {}", __func__, name);
            return name;
        }
    }

    const auto msg = fmt::format("Failed to open resource {}", resourceName);
    print::error(msg);
    throw std::invalid_argument{msg};
}

std::stringstream AssetManager::openResource(std::string_view resourceName) {
    print::info("{}: {}", __func__, resourceName);

    std::ifstream file{getActualFileName(resourceName)};

    std::stringstream str;
    str << file.rdbuf();
    return str;
}

void AssetManager::loadTextures(TextureSet& toLoad) {
    print::info("Loading {} textures", toLoad.size());
    for (const auto t: toLoad) {
        auto [it, emplaced] = textures_.emplace(t, sf::Texture{});
        if (!emplaced) continue;
        const auto filename = texToFilename.find(t);
        if (filename == texToFilename.end()) {
            const auto msg = fmt::format("Texture {} does not have an associated resource name.", t);
            print::error(msg);
            throw std::invalid_argument{msg};
        }
        it->second.loadFromFile(getActualFileName(filename->second));
        it->second.setSmooth(true);
    }
}

void AssetManager::releaseTextures(TextureSet& toRelease) {
    print::info("Releasing {} textures", toRelease.size());
    for (auto t: toRelease) {
        textures_.erase(t);
    }
}

const sf::Texture& AssetManager::getTexture(TextureType t) {
    const auto kvPair = textures_.find(t);
    if (kvPair == textures_.end()) {
        const auto msg = fmt::format("Texture {} not loaded.", t);
        print::error(msg);
        throw std::invalid_argument{msg};
    }
    return kvPair->second;
}
