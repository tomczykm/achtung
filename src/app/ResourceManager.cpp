#include "app/ResourceManager.hpp"

#include <array>
#include <fstream>
#include <string>
#include <stdexcept>

#include <fmt/format.h>

#include "app/Log.hpp"

namespace ResourceManager {

std::stringstream openResource(std::string_view resourceName) {
    print::info("{}: {}", __func__, resourceName);

    const std::array<std::string, 4> fileNames = {
        fmt::format("res/{}", resourceName),
        resourceName.data(),
        fmt::format("../res/{}", resourceName),  // for running from build directory
        fmt::format("../{}", resourceName)
    };

    std::ifstream file;
    for (const std::string_view name: fileNames) {
        file.open(name.data());
        if (file) {
            print::info("{}: success! the filename is {}", __func__, name);
            break;
        }
    }

    if (!file) throw std::invalid_argument{resourceName.data()};

    std::stringstream str;
    str << file.rdbuf();
    return str;
}

}  // namespace ResourceManager