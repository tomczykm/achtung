#pragma once

#include "BaseGameState.h"
#include "CTexture.h"
#include "Global.h"
#include "PlayerThing.h"

#include <vector>

enum EGameplayStatus {
	STATUS_NULL = 0,
	STATUS_ROUND_BEGIN, //at the start of each round; move to RUNNING
	STATUS_RUNNING, //when everybody is furiously trying to kill each other; move to PAUSE, ROUND_END or GAME_END
	STATUS_PAUSE, //when a player needs a break; move to RUNNING or quit the game in progress
	STATUS_ROUND_END, //when there's only one standing; move to ROUND_BEGIN
	STATUS_GAME_END //when a player has reached the score goal and is declared a winner; move to another gamestate (TODO)
};

class CStateTest: public CBaseGameState {
public:
	CStateTest ();
	~CStateTest ();

	void Input();
	void Logic();
	void Render();
private:
	CTexture playerTex;
	CTexture wallTex;

	//dimensions for border
	//cannot be static const as they use SettingsHandler::GetResH() to initialize
	const int wt, bx, bw;

	std::vector <CPlayerThing> players;

	Uint32 timer;
};
