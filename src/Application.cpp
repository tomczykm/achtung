#include <cstdlib>
#include <ctime>
#include <SDL_image.h>

#include "Application.hpp"
#include "Global.hpp"

#include "Game/StateGame.hpp"
#include "Game/StateSandbox.hpp"
#include "StateSettings.hpp"

Application::Application():
    window_(nullptr),
    renderer_(nullptr),
    gameState_(),
    changeState_(nullptr),
    quit_(false),
    projectName_(PROJECT_NAME)
{}

Application::~Application() {
    logger->out("Cleanup");
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool Application::init() {
    logger = std::make_unique<Logger>();
    logger->out("Starting initialization...");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        logger->error("FATAL: Failed to init SDL video. SDL_Error: " + std::string(SDL_GetError()));
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        logger->out("WARNING: Linear filtering is OFF!");
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
        logger->out("WARNING: VSync is OFF!");
    }

    window_ = SDL_CreateWindow(projectName_.c_str(), SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, settings_.getResW(), settings_.getResH(),
        SDL_WINDOW_SHOWN | (settings_.getFullscreen() ? SDL_WINDOW_FULLSCREEN : 0));

    if (window_ == nullptr) {
        logger->error("FATAL: Failed to create a window. SDL_Error: " + std::string(SDL_GetError()));
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        logger->error("FATAL: Failed to initialize SDL_Image. IMG_Error: " + std::string(IMG_GetError()));
        return false;
    }

    std::srand(std::time(nullptr));

    logger->out("Creating gamestate");
    gameState_.reset(new StateSandbox(*this));

    logger->out("Initialization complete!");
    return true;
}

int Application::run() {
    logger->out("Starting main loop...");
    while (!quit_) {
        while (SDL_PollEvent(&events_)) {
            if ((events_.type == SDL_QUIT)) {
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
