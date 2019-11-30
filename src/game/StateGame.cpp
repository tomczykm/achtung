#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

#include <algorithm>

#include "Utils.hpp"

StateGame::StateGame(const Application::Interface& ctx):
    app_(ctx)
{
    players_.emplace_back("player"/*, SDL_SCANCODE_Q, SDL_SCANCODE_W*/);
    for (auto &p: players_) {
        p.newRoundSetup(100, 600, 100, 600);
    }
    lastAlive_ = players_.end();
}

// void StateGame::input() {
//     for (auto it = players_.begin() ; it != lastAlive_ ; it++) {
//         it->input();
//     }
// }

void StateGame::input(const sf::Event& event) {
    // if (app_.events.type == SDL_KEYDOWN) {
    //     switch(app_.events.key.keysym.sym) {
    //     case SDLK_SPACE:
    //         // space (hehe) for shitty debugging
    //         app_.enterState<StateSandbox>();
    //     }
    // }
}

void StateGame::logic() {
    bool playerDied = false;

    for (auto it = players_.begin() ; it != lastAlive_ ; it++) {
        // it->move((SDL_GetTicks() - moveTimer_) / 1000.f);
        it->createTrail(trails_);

        for (const auto& t: trails_) {
            if (!it->isGap() && it->checkCollision(t)) {
                it->kill();
                playerDied = true;
            }
        }
    }

    if (playerDied) {
        lastAlive_ = std::partition(players_.begin(), players_.end(),
            [] (auto& p) { return !p.isDead(); });
    }

    // moveTimer_ = SDL_GetTicks();
}

void StateGame::render() {
    // // draw trails
    // for (auto& t: trails_) {
    //     auto rec = t.getRenderRect();
    //     trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    // }

    // // draw players
    // for (auto& p: players_) {
    //     const auto& rec = p.getRenderRect();
    //     playerTex_.render(rec.x, rec.y, rec.w, rec.h);
    // }

    // // drawing outer walls
    // auto renderer = &app_.renderer;
    // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    // SDL_Rect rect;
    // // top
    // rect = { bx_, bx_, bw_, wt_ };
    // SDL_RenderFillRect(renderer, &rect);
    // // bottom
    // rect = { bx_, bx_+bw_-wt_, bw_, wt_ };
    // SDL_RenderFillRect(renderer, &rect);
    // // left
    // rect = { bx_, bx_, wt_, bw_ };
    // SDL_RenderFillRect(renderer, &rect);
    // // right
    // rect = { bx_+bw_-wt_, bx_, wt_, bw_ };
    // SDL_RenderFillRect(renderer, &rect);
    // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}
