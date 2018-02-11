#include "inc/BaseGameState.h"
#include "inc/Global.h"
#include "inc/StateTest.h"
#include "inc/StateSettings.h"

#include <cstdlib>

void CBaseGameState::SetNextState( EGameState newState ) {
	if ( nextState != STATE_EXIT ) {
		nextState = newState;
	}
}

void CBaseGameState::ChangeState() {
	if ( nextState != STATE_NULL ) {
		if ( nextState != STATE_EXIT ) {
			logger->Out( "Changing current game state..." );
			delete currentGameState;
		}
		switch( nextState ) {
		case STATE_TEST:
			currentGameState = new CStateTest();
			break;
		case STATE_SETTINGS:
			currentGameState = new CStateSettings();
		case STATE_EXIT:
			logger->Out( "Quitting the game" );
			break;
    case STATE_NULL:
		default:
			logger->Error( "FATAL: Attempt to enter undefined gamestate." );
			exit( -1 );
		}
		stateID = nextState;
		nextState = STATE_NULL;
	}
}
