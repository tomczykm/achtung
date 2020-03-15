#include "app/ResourceManager.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include <fmt/format.h>

#include "app/Log.hpp"

namespace ResourceManager {

std::string getActualFileName(std::string_view resourceName) {
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

std::stringstream openResource(std::string_view resourceName) {
    print::info("{}: {}", __func__, resourceName);

    std::ifstream file{getActualFileName(resourceName)};

    std::stringstream str;
    str << file.rdbuf();
    return str;
}

}  // namespace ResourceManager
