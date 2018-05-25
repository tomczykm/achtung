#include "BaseGameState.h"
#include "Global.h"
#include "StateGame.h"
#include "StateSettings.h"

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
			currentGameState.reset( new StateGame() );
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
