#include "app/AssetManager.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

#include "app/Log.hpp"

namespace {

const std::map<AssetManager::Texture, std::string_view> texToFilename = {
    {AssetManager::Texture::SelfHaste, "selfHaste.png"},
    {AssetManager::Texture::OpponentHaste, "oppHaste.png"},
    {AssetManager::Texture::SelfSlow, "selfSlow.png"},
    {AssetManager::Texture::OpponentSlow, "oppSlow.png"},
    {AssetManager::Texture::ClearTrails, "clear.png"},
    {AssetManager::Texture::RandomPickMeUp, "random.png"}
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

    throw std::invalid_argument{resourceName.data()};
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
        textures_[t].loadFromFile(getActualFileName(texToFilename.find(t)->second));
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
    const auto found = textures_.find(t);
    if (found == textures_.end()) throw std::invalid_argument{fmt::format("Invalid texture {}", t)};
    return found->second;
}
