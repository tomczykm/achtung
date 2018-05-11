#include <SDL.h>
#include <SDL_image.h>

#include "Global.h"
#include "StateTest.h"

bool Init();
void Cleanup();

bool Init() {
	logger = std::make_unique< Logger >();
	logger->Out( "Starting initialization..." );
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		logger->Error( "FATAL: Failed to init SDL video. SDL_Error: " + std::string( SDL_GetError() ) );
		return false;
	}
	logger->Out( "SDL_Init() OK" );

	if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
		logger->Out( "WARNING: Linear filtering is OFF!" );
	}

	if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) ) {
		logger->Out( "WARNING: VSync is OFF!" );
	}

	settings = std::make_unique< SettingsHandler >();

	window = SDL_CreateWindow( projectName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings->GetResW(), settings->GetResH(), SDL_WINDOW_SHOWN | ( settings->GetFullscreen() ? SDL_WINDOW_FULLSCREEN : 0 ) );
	if( window == nullptr ) {
		logger->Error( "FATAL: Failed to create a window. SDL_Error: " + std::string( SDL_GetError() ) );
		return false;
	}
	logger->Out( "SDL_CreateWindow() OK" );

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

  int imgFlags = IMG_INIT_PNG;
  if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
    logger->Error( "FATAL: Failed to initialize SDL_Image. IMG_Error: " + std::string( IMG_GetError() ) );
    return false;
  }

	std::srand( std::time( nullptr ) );

	logger->Out( "Creating test game state" );
	currentGameState.reset( new StateTest() );

	logger->Out( "Initialization complete!" );
	return true;
}

void Cleanup() {
	logger->Out( "Cleanup..." );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
}

int main( int argc, char **argv ) {
	if( !Init() ) {
		logger->Out( "Initialization failed!" );
		return -1;
	}

	logger->Out( "Starting main loop..." );
	while( stateID != GameState::quit ) {
		//input
		while ( SDL_PollEvent( &events ) ) {
			if ( ( events.type == SDL_QUIT ) ) {
				BaseGameState::SetNextState( GameState::quit );
			}
			currentGameState->PolledInput();
		}
		currentGameState->Input();

		//logic
		currentGameState->Logic();
		BaseGameState::ChangeState();

		//rendering
		SDL_RenderClear( renderer );
		currentGameState->Render();
		SDL_RenderPresent( renderer );
	}

	Cleanup();
	return 0;
}
