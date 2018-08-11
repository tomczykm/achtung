#include "StateSandbox.h"
#include "Global.h"

StateSandbox::StateSandbox():
    playerTex_("dot.png"),
    trailTex_("wall.png"),
    player_("player", SDL_SCANCODE_Q, SDL_SCANCODE_W),
    move_(false),
    moveTimer_(SDL_GetTicks())
{
    player_.newRoundSetup(100, 600, 100, 600);
}

StateSandbox::~StateSandbox() {

}

void StateSandbox::input() {
    player_.input();
}

void StateSandbox::polledInput() {
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
        player_.move(( SDL_GetTicks() - moveTimer_ ) / 1000.f);
    }

    for (const auto& t: trails_) {
        if (player_.checkCollision( t )) {
            logger->out("Collsion!");
        }
    }
    moveTimer_ = SDL_GetTicks();
}

void StateSandbox::render() {
    for (auto &t: trails_) {
        auto rec = t.getRenderRect();
        trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    }

    auto rec = player_.getRenderRect();
    playerTex_.render(rec.x, rec.y, rec.w, rec.h);
}
