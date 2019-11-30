#include <cstdio>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Application.hpp"
#include "Log.hpp"

namespace print::detail
{
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

int main(int argc, char **argv) {
    print::detail::logFile.reset(std::fopen("event.log", "w"));
    Application app;
    if (!app.init()) {
        print::error("Initialization failed");
        return -1;
    }

    return app.run();
}
