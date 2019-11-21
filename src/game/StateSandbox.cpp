#include "game/StateSandbox.hpp"
#include "Log.hpp"

StateSandbox::StateSandbox(Application& app):
    app_(app)
{
    player_.newRoundSetup(100, 600, 100, 600);
}

void StateSandbox::input() {
    player_.input();
}

void StateSandbox::polledInput() {
    auto& events = app_.events();

    if (events.type == SDL_KEYDOWN) {
        switch(events.key.keysym.sym) {
        case SDLK_SPACE:
            move_ = true;
        }
    }

    if (events.type == SDL_KEYUP) {
        switch(events.key.keysym.sym) {
        case SDLK_SPACE:
            move_ = false;
        }
    }
}


void StateSandbox::logic() {
    if (move_) {
        player_.move((SDL_GetTicks() - moveTimer_) / 1000.f);
    }

    for (const auto& t: trails_) {
        if (player_.checkCollision(t)) {
            log_ << info << "Collision!";
        }
    }
    moveTimer_ = SDL_GetTicks();
}

void StateSandbox::render() {
    for (auto& t: trails_) {
        const auto& rec = t.getRenderRect();
        trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    }

    const auto& rec = player_.getRenderRect();
    playerTex_.render(rec.x, rec.y, rec.w, rec.h);
}
