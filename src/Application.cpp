#include <cstdlib>
#include <ctime>
#include <SDL_image.h>

#include "Application.hpp"
#include "Log.hpp"

#include "Game/StateGame.hpp"
#include "Game/StateSandbox.hpp"

Application::Application():
    window_(nullptr),
    renderer_(nullptr),
    gameState_(),
    changeState_(nullptr),
    quit_(false),
    projectName_(PROJECT_NAME)
{}

Application::~Application() {
    log_ << info << "Cleanup";
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool Application::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        log_ << error << "Failed to init SDL video. SDL_Error: " << SDL_GetError();
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        log_ << warning << "Linear filtering is off";
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
        log_ << warning << "VSync is off";
    }

    window_ = SDL_CreateWindow(projectName_.c_str(), SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, settings_.getResW(), settings_.getResH(),
        SDL_WINDOW_SHOWN | (settings_.getFullscreen() ? SDL_WINDOW_FULLSCREEN : 0));

    if (window_ == nullptr) {
        log_ << error << "Failed to create a window. SDL_Error: " << SDL_GetError();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        log_ << error << "Failed to initialize SDL_Image. IMG_Error: " << IMG_GetError();
        return false;
    }

    std::srand(std::time(nullptr));

    gameState_ = std::make_unique<StateSandbox>(*this);

    log_ << info << "Initialization complete";
    return true;
}

int Application::run() {
    while (!quit_) {
        while (SDL_PollEvent(&events_)) {
            if (events_.type == SDL_QUIT) {
                quitGame();
            }
            gameState_->polledInput();
        }
        gameState_->input();

        gameState_->logic();
        if(changeState_ != nullptr) {
            changeState_();
            changeState_ = nullptr;
        }

        SDL_RenderClear(renderer_);
        gameState_->render();
        SDL_RenderPresent(renderer_);
    }
    return 0;
}
