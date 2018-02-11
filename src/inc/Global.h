#pragma once

#include <SDL.h>
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

extern SDL_Window		    *window;
extern CLogger			    *logger;
extern CSettingsHandler		*settings;
extern SDL_Event		    events;

extern CBaseGameState		*currentGameState;
extern EGameState		    stateID,
                            nextState;

/**
	From BaseGameState.cpp
**/

extern void     SetNextState( EGameState newState );
extern void     ChangeState();

/**
    Z Global.cpp
**/

template < typename T >
std::string ToString( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

