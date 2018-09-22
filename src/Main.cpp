#include "Application.hpp"
#include "Global.hpp"

int main(int argc, char **argv) {
    Application app;
    if (!app.init()) {
        logger->out("Initialization failed");
        return -1;
    }

    return app.run();
}
