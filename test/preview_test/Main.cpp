#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory>

#include "app/Application.hpp"
#include "app/Log.hpp"
#include "preview_test/StateViewer.hpp"

namespace print::detail
{
std::unique_ptr<std::FILE, FCloseDeleter> logFile;
}

int main(int argc, char** argv) {
    print::detail::logFile.reset(std::fopen("event.log", "w"));
    {
        const auto time = std::time(nullptr);
        xor_rand::seed(time, time >> 8, time << 8, time >> 16);
    }

    if (argc == 1) {
        return 1;
    }

    const auto testcaseNum = std::atoi(argv[1]);

    return Application::makeApplication<StateViewer>(testcaseNum)->run();
}
