#include "StateGame.h"

#include <algorithm>

StateGame::StateGame():
    playerTex_("dot.png"),
    trailTex_("wall.png"),
    wt_(4),
    bx_(0.05 * settings->getResH()),
    bw_(0.9 * settings->getResH()),
    moveTimer_(SDL_GetTicks()),
    status_(GameplayStatus::roundBegin)
{
    players_.emplace_back("player", SDL_SCANCODE_Q, SDL_SCANCODE_W);
    for (auto &p: players_) {
        p.newRoundSetup(100, 600, 100, 600);
    }
}

StateGame::~StateGame() {

}

void StateGame::input() {
    for (auto &p: players_) {
        p.input();
    }
}

void StateGame::polledInput() {
    if (events.type == SDL_KEYDOWN) {
        switch(events.key.keysym.sym) {
        case SDLK_SPACE:
            //space (hehe) for shitty debugging
            ;
        }
    }
}

void StateGame::logic() {
    auto endAlive = std::partition(players_.begin(), players_.end(),
        [](const PlayerThing &p ) { return !p.isDead(); });

    for (auto it = players_.begin() ; it != endAlive ; it++) {
        it->move((SDL_GetTicks() - moveTimer_) / 1000.f);
        it->createTrail(trails_);

        for (const auto& t: trails_) {
            if (it->checkCollision(t) && !it->isGap()) {
                it->die();
            }
        }
    }
    moveTimer_ = SDL_GetTicks();
}

void StateGame::render() {
    //draw trails
    for (auto &t: trails_) {
        auto rec = t.getRenderRect();
        trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    }

    //draw players
    for (auto &p: players_) {
        auto rec = p.getRenderRect();
        playerTex_.render(rec.x, rec.y, rec.w, rec.h);
    }

    //drawing outer walls
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    SDL_Rect rect;
    //top
    rect = { bx_, bx_, bw_, wt_ };
    SDL_RenderFillRect(renderer, &rect);
    //bottom
    rect = { bx_, bx_+bw_-wt_, bw_, wt_ };
    SDL_RenderFillRect(renderer, &rect);
    //left
    rect = { bx_, bx_, wt_, bw_ };
    SDL_RenderFillRect(renderer, &rect);
    //right
    rect = { bx_+bw_-wt_, bx_, wt_, bw_ };
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}
