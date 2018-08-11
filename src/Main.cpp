#include <SDL.h>
#include <SDL_image.h>

#include "Global.h"
#include "StateGame.h"
#include "StateSandbox.h"

bool init() {
    logger = std::make_unique<Logger>();
    logger->out("Starting initialization...");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        logger->error("FATAL: Failed to init SDL video. SDL_Error: " + std::string(SDL_GetError()));
        return false;
    }
    logger->out("SDL_Init() OK");

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        logger->out("WARNING: Linear filtering is OFF!");
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
        logger->out("WARNING: VSync is OFF!");
    }

    settings = std::make_unique<SettingsHandler>();

    window = SDL_CreateWindow(projectName.c_str(), SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, settings->getResW(), settings->getResH(),
        SDL_WINDOW_SHOWN | (settings->getFullscreen() ? SDL_WINDOW_FULLSCREEN : 0));

    if (window == nullptr) {
        logger->error("FATAL: Failed to create a window. SDL_Error: " + std::string(SDL_GetError()));
        return false;
    }
    logger->out("SDL_CreateWindow() OK");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        logger->error("FATAL: Failed to initialize SDL_Image. IMG_Error: " + std::string(IMG_GetError()));
        return false;
    }

    std::srand(std::time(nullptr));

    logger->out("Creating gamestate");
    currentGameState.reset(new StateSandbox());

    logger->out("Initialization complete!");
    return true;
}

void cleanup() {
    logger->out("Cleanup...");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char **argv) {
    if (!init()) {
        logger->out("Initialization failed");
        return -1;
    }

    logger->out("Starting main loop...");
    while (stateID != GameState::quit) {
        //input
        while (SDL_PollEvent(&events)) {
            if ((events.type == SDL_QUIT)) {
                BaseGameState::setNextState(GameState::quit);
            }
            currentGameState->polledInput();
        }
        currentGameState->input();

        //logic
        currentGameState->logic();
        BaseGameState::changeState();

        //rendering
        SDL_RenderClear(renderer);
        currentGameState->render();
        SDL_RenderPresent(renderer);
    }

    cleanup();
    return 0;
}
