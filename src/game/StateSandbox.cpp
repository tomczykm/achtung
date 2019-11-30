#include "game/StateSandbox.hpp"
#include "game/StateGame.hpp"
#include "Log.hpp"

StateSandbox::StateSandbox(const Application::Interface& ctx):
    app_(ctx)
{
    player_.newRoundSetup(100, 600, 100, 600);
    print::info("StateSandbox ready");
}

void StateSandbox::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            move_ = true;
            break;
        case sf::Keyboard::T:
            app_.enterState<StateGame>();
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
    }

    // for (const auto& t: trails_) {
    //     if (player_.checkCollision(t)) {
    //         print::info("Collision!");
    //     }
    // }
    moveTimer_.restart();
}

void StateSandbox::render() {
    // for (auto& t: trails_) {
    //     const auto& rec = t.getRenderRect();
    //     trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    // }

    app_.window.draw(player_.getShape());
}
