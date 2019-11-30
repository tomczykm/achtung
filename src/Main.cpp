#include <cstdio>
#include <memory>

#include "Application.hpp"
#include "Log.hpp"

namespace print::detail
{
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

int main(int argc, char **argv) {
    print::detail::logFile.reset(std::fopen("event.log", "w"));
    std::srand(std::time(nullptr));
    Application app;
    return app.run();
}
