#include "inc/BaseGameState.h"
#include "inc/Global.h"
#include "inc/StateTest.h"
#include "inc/StateSettings.h"

#include <cstdlib>

void BaseGameState::SetNextState( GameState newState ) {
	if ( nextState != GameState::quit ) {
		nextState = newState;
	}
}

void BaseGameState::ChangeState() {
	if ( nextState != GameState::null ) {
		if ( nextState != GameState::quit ) {
			logger->Out( "Changing current game state..." );
		}
		switch( nextState ) {
		case GameState::test:
			currentGameState.reset( new StateTest() );
			break;
		case GameState::settings:
			currentGameState.reset( new StateSettings() );
			break;
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
