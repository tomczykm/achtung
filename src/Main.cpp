#include <cstdio>
#include <ctime>
#include <memory>

#include "app/Application.hpp"
#include "app/Log.hpp"

namespace print::detail
{
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

int main() {
    print::detail::logFile.reset(std::fopen("event.log", "w"));
    {
        const auto time = std::time(nullptr);
        xor_rand::seed(time, time >> 8, time << 8, time >> 16);
    }
    Application app;
    return app.run();
}
