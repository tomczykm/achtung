#pragma once

#include <any>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include "app/Log.hpp"

enum class Setting {
    Null = 0,

    TickRate,

    ResWidth,
    ResHeight,
    Fullscreen
};

class ConfigManager {
public:
    ConfigManager();

    template <typename T>
    auto get(Setting key) {
        const auto found = values_.find(key);
        if (found == values_.end()) throw std::invalid_argument{fmt::format("{}: Invalid key {}", __func__, key)};
        return std::any_cast<T>(found->second);
    }

    void saveToFile();

private:
    void loadFromFile();

    std::string filename_;

    std::unordered_map<Setting, std::any> values_;
};
