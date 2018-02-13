#pragma once

#include "BaseGameState.h"

class CStateSettings: public CBaseGameState {
public:
	CStateSettings ();
	~CStateSettings ();

	void Input();
	void Logic();
	void Render();
	void PolledInput();
private:
};
