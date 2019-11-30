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
    // if (app_.events.type == SDL_KEYDOWN) {
    //     switch(app_.events.key.keysym.sym) {
    //     case SDLK_SPACE:
    //         move_ = true;
    //         break;
    //     case SDLK_t:
    //         app_.enterState<StateGame>();
    //         break;
    //     }
    // }

    // if (app_.events.type == SDL_KEYUP) {
    //     switch(app_.events.key.keysym.sym) {
    //     case SDLK_SPACE:
    //         move_ = false;
    //         break;
    //     }
    // }
}

void StateSandbox::logic() {
    // if (move_) {
    //     player_.move((SDL_GetTicks() - moveTimer_) / 1000.f);
    // }

    // for (const auto& t: trails_) {
    //     if (player_.checkCollision(t)) {
    //         print::info("Collision!");
    //     }
    // }
    // moveTimer_ = SDL_GetTicks();
}

void StateSandbox::render() {
    // for (auto& t: trails_) {
    //     const auto& rec = t.getRenderRect();
    //     trailTex_.render(rec.x, rec.y, rec.w, rec.h, t.getAngle());
    // }

    app_.window.draw(player_.getShape());
}
