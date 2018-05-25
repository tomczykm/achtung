#pragma once

#include "BaseGameState.h"

class StateSettings: public BaseGameState {
public:
	StateSettings ();
	~StateSettings ();

	void Input();
	void Logic();
	void Render();
	void PolledInput();
private:
};
