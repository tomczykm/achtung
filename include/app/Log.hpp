#pragma once

#include <cstdio>
#include <memory>
#include <string_view>
#include <fmt/format.h>

namespace print::detail
{

struct FCloseDeleter {
    void operator()(std::FILE* f) {
        std::fclose(f);
    }
};

extern std::unique_ptr<std::FILE, FCloseDeleter> logFile;

template <typename... Ts>
void printInternal(std::string_view prefix, std::string_view formatString, Ts&&... params) {
    const auto completePrint = fmt::format("{} {} {}\n", time(nullptr),
        prefix, fmt::format(formatString, std::forward<Ts>(params)...));
    fmt::print(completePrint);
    fmt::print(logFile.get(), completePrint);
}

}  // namespace print::detail

namespace print
{

template <typename... Ts>
void info(std::string_view formatString, Ts&&... params) {
    detail::printInternal("info:", formatString, std::forward<Ts>(params)...);
}

template <typename... Ts>
void error(std::string_view formatString, Ts&&... params) {
    detail::printInternal("Error!", formatString, std::forward<Ts>(params)...);
}

}  // namespace print
