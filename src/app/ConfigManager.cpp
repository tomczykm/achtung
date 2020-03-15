#include "app/ConfigManager.hpp"

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
    const auto video = toml::find(parsed, "video");

    values_.emplace(Setting::ResWidth, toml::find_or<int>(video, "res_width", 1280));
    values_.emplace(Setting::ResHeight, toml::find_or<int>(video, "res_height", 720));
    values_.emplace(Setting::Fullscreen, toml::find_or<bool>(video, "fullscreen", false));
}

void ConfigManager::saveToFile() {

}
