#include "app/ConfigManager.hpp"

#include <cstdint>
#include <toml.hpp>

#include "app/AssetManager.hpp"
#include "app/Log.hpp"

ConfigManager::ConfigManager():
    filename_{AssetManager::getActualFileName("config.toml")}
{
    loadFromFile();
}

void ConfigManager::loadFromFile() {
    const auto parsed = toml::parse(filename_);

    const auto game = toml::find(parsed, "game");
    values_.emplace(Setting::TickRate, toml::find_or<int>(game, "tickrate", 140));

    const auto video = toml::find(parsed, "video");

    const auto resW = toml::find_or<int>(video, "res_width", 1280);
    const auto resH = toml::find_or<std::uint32_t>(video, "res_height", 720);

    values_.emplace(Setting::ResWidth, static_cast<uint32_t>(resW));
    values_.emplace(Setting::ResHeight, static_cast<uint32_t>(resH));
    values_.emplace(Setting::Fullscreen, toml::find_or<bool>(video, "fullscreen", false));
}

void ConfigManager::saveToFile() {

}
