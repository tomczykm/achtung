#include "game/StateSandbox.hpp"
#include "menu/StateMenu.hpp"
#include "Log.hpp"

StateSandbox::StateSandbox(const Application::Interface& ctx):
    app_{ctx},
    player_{{"player", sf::Keyboard::Q, sf::Keyboard::W, sf::Color::Red}}
{
    player_.newRoundSetup(100, 600, 100, 600, trails_);
    print::info("StateSandbox ready");
}

void StateSandbox::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            move_ = true;
            break;
        case sf::Keyboard::Escape:
            app_.enterState<StateMenu>();
            break;
        default: break;
        }
    }

    if (event.type == sf::Event::KeyReleased) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            move_ = false;
            break;
        default: break;
        }
    }
}

void StateSandbox::logic() {
    if (move_) {
        player_.move(moveTimer_.getElapsedTime().asMilliseconds() / 1000.f);
        player_.createTrail(trails_);
    }

    constexpr auto SKIP_TRAILS = 15u;
    if (trails_.size() > SKIP_TRAILS) {
        for (auto t = trails_.begin() + SKIP_TRAILS; t != trails_.end(); t++) {
            if (player_.checkCollision(t->getShape())) {
                t->setColor(sf::Color::Red);
            }
        }
    }
    moveTimer_.restart();
}

void StateSandbox::render() {
    for (const auto& t: trails_) {
        app_.window.draw(t.getShape());
    }

    app_.window.draw(player_.getShape());
}
