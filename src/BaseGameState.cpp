#include "inc/BaseGameState.h"
#include "inc/Global.h"
#include "inc/StateTest.h"
#include "inc/StateSettings.h"

#include <cstdlib>

void CBaseGameState::SetNextState( GameState newState ) {
	if ( nextState != GameState::quit ) {
		nextState = newState;
	}
}

void CBaseGameState::ChangeState() {
	if ( nextState != GameState::null ) {
		if ( nextState != GameState::quit ) {
			logger->Out( "Changing current game state..." );
			delete currentGameState;
		}
		switch( nextState ) {
		case GameState::test:
			currentGameState = new CStateTest();
			break;
		case GameState::settings:
			currentGameState = new CStateSettings();
		case GameState::quit:
			logger->Out( "Quitting the game" );
			break;
    case GameState::null:
		default:
			logger->Error( "FATAL: Attempt to enter undefined gamestate." );
			exit( -1 );
		}
		stateID = nextState;
		nextState = GameState::null;
	}
}
