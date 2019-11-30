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
    // Application app;
    // if (!app.init()) {
    //     print::error("Initialization failed");
    //     return -1;
    // }

    // return app.run();
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}
