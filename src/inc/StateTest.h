#pragma once

#include "BaseGameState.h"

class CStateTest: public CBaseGameState {
public:
	CStateTest	();
	~CStateTest	();

	void		Input();
	void		Logic();
	void		Render();
private:
};

