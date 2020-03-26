#include "app/AssetManager.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

#include "app/Log.hpp"

namespace {

const std::map<AssetManager::Texture, std::string_view> texToFilename = {
    {AssetManager::Texture::SelfHaste, "sprites/selfHaste.png"},
    {AssetManager::Texture::OpponentHaste, "sprites/oppHaste.png"},
    {AssetManager::Texture::SelfSlow, "sprites/selfSlow.png"},
    {AssetManager::Texture::OpponentSlow, "sprites/oppSlow.png"},
    {AssetManager::Texture::SelfRightAngle, "sprites/selfSquare.png"},
    {AssetManager::Texture::OpponentRightAngle, "sprites/oppSquare.png"},
    {AssetManager::Texture::ClearTrails, "sprites/clear.png"},
    {AssetManager::Texture::ControlSwap, "sprites/swap.png"},
    {AssetManager::Texture::MassPowerups, "sprites/massPowerups.png"},
    {AssetManager::Texture::RandomPickMeUp, "sprites/random.png"}
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
        textures_.emplace(t, sf::Texture{});
        const auto filename = texToFilename.find(t);
        if (filename == texToFilename.end()) {
            const auto msg = fmt::format("Texture {} does not have an associated resource name.", t);
            print::error(msg);
            throw std::invalid_argument{msg};
        }
        textures_[t].loadFromFile(getActualFileName(filename->second));
        textures_[t].setSmooth(true);
    }
}

void AssetManager::releaseTextures(TextureSet& toRelease) {
    print::info("Releasing {} textures", toRelease.size());
    for (auto t: toRelease) {
        textures_.erase(t);
    }
}

const sf::Texture& AssetManager::getTexture(Texture t) {
    const auto kvPair = textures_.find(t);
    if (kvPair == textures_.end()) {
        const auto msg = fmt::format("Texture {} not loaded.", t);
        print::error(msg);
        throw std::invalid_argument{msg};
    }
    return kvPair->second;
}
