#include "game/StateGame.hpp"

#include <algorithm>

#include "Utils.hpp"

StateGame::StateGame(const Application::Interface& ctx):
    app_(ctx),
    border_(0.05 * app_.settings.getResH(), 0.9 * app_.settings.getResH())
{
    players_.emplace_back("player", sf::Keyboard::Q, sf::Keyboard::W);
    for (auto &p: players_) {
        p.newRoundSetup(100, 600, 100, 600, trails_);
    }
    lastAlive_ = players_.end();
    app_.window.setMouseCursorVisible(false);

    print::info("StateGame ready");
}

StateGame::~StateGame() {
    app_.window.setMouseCursorVisible(true);
}

void StateGame::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            // space (hehe) for shitty debugging
            break;
        default: break;
        }
    }
}

void StateGame::logic() {
    bool playerDied = false;

    for (auto player = players_.begin() ; player != lastAlive_ ; player++) {
        player->move(moveTimer_.getElapsedTime().asMilliseconds() / 1000.f);
        player->createTrail(trails_);

        for (const auto& shape: border_.getShapes()) {
            if (!player->isGap() && player->checkCollision(shape)) {
                player->kill();
                playerDied = true;
            }
        }

        // this is fairly shoddy, better come up with a better idea on skipping later

        // the reason we skip the newest bunch of trail segments is that
        // we don't want to check if we collide with segments we just created
        // (we obviously do, they're right underneath and we don't want to die immediately)
        constexpr auto SKIP_TRAILS = 15u;
        if (trails_.size() > SKIP_TRAILS) {
            for (auto t = trails_.begin() + SKIP_TRAILS; t != trails_.end(); t++) {
                if (!player->isGap() && player->checkCollision(t->getShape())) {
                    player->kill();
                    playerDied = true;
                }
            }
        }
    }

    if (playerDied) {
        lastAlive_ = std::partition(players_.begin(), players_.end(),
            [] (auto& p) { return !p.isDead(); });
    }

    moveTimer_.restart();
}

void StateGame::render() {
    for (const auto& t: trails_) {
        app_.window.draw(t.getShape());
    }

    for (const auto& shape: border_.getShapes()) {
        app_.window.draw(shape);
    }

    for (const auto& p: players_) {
        app_.window.draw(p.getShape());
    }
}
