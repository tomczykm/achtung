#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <memory>

#include "CMakeDefine.h"
#include "Logger.h"
#include "SettingsHandler.h"
#include "BaseGameState.h"

/**
  From Global.cpp
**/

extern int RandomInt( int min, int max );

extern const std::string projectName;

extern SDL_Window *window;
extern SDL_Event events;
extern SDL_Renderer *renderer;

extern std::unique_ptr< Logger > logger;
extern std::unique_ptr< SettingsHandler > settings;

extern std::unique_ptr< BaseGameState > currentGameState;
extern GameState stateID, nextState;

/**
	From BaseGameState.cpp
**/

extern void SetNextState( GameState newState );
extern void ChangeState();
