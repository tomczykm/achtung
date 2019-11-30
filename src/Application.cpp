#include <cstdlib>
#include <ctime>
#include <SDL_image.h>

#include "Application.hpp"
#include "Log.hpp"

#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

Application::~Application() {
    print::info("Cleanup");
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    IMG_Quit();
    SDL_Quit();
}

bool Application::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        print::error("Failed to init SDL video. SDL_Error: {}", SDL_GetError());
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        print::info("Linear filtering is off");
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
        print::info("VSync is off");
    }

    window_ = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, settings_.getResW(), settings_.getResH(),
        SDL_WINDOW_SHOWN | (settings_.getFullscreen() ? SDL_WINDOW_FULLSCREEN : 0));

    if (!window_) {
        print::error("Failed to create a window. SDL_Error: {}", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        print::error("Failed to initialize SDL_Image. IMG_Error: {}", IMG_GetError());
        return false;
    }

    std::srand(std::time(nullptr));

    gameState_ = std::make_unique<StateSandbox>(Interface{*this});

    print::info("Initialization complete");
    return true;
}

int Application::run() {
    while (!quit_) {
        while (SDL_PollEvent(&events_)) {
            if (events_.type == SDL_QUIT) {
                quit_ = true;
            }
            gameState_->polledInput();
        }
        gameState_->input();

        gameState_->logic();
        if (changeState_) {
            changeState_();
            changeState_ = nullptr;
        }

        SDL_RenderClear(renderer_);
        gameState_->render();
        SDL_RenderPresent(renderer_);
    }
    return 0;
}
