#include "Application.hpp"
#include "Log.hpp"

int main(int argc, char **argv) {
    Application app;
    if (!app.init()) {
        log_ << error <<"Initialization failed";
        return -1;
    }

    return app.run();
}
