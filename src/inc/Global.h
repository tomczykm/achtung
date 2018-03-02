#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "Logger.h"
#include "SettingsHandler.h"
#include "BaseGameState.h"

/**
	From Main.cpp
**/

extern SDL_Window *window;
extern CLogger *logger;
extern CSettingsHandler	*settings;
extern SDL_Event events;
extern SDL_Renderer *renderer;

extern CBaseGameState *currentGameState;
extern GameState stateID, nextState;

/**
	From BaseGameState.cpp
**/

extern void SetNextState( GameState newState );
extern void ChangeState();

/**
    Z Global.cpp
**/

extern int RandomInt( int min, int max );

template < typename T >
std::string ToString( const T& n )
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}
