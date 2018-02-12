#pragma once

#include "BaseGameState.h"
#include "CTexture.h"

class CStateTest: public CBaseGameState {
public:
	CStateTest		();
	~CStateTest		();

	void					Input();
	void					Logic();
	void					Render();
private:

	CTexture			tex;
	CTexture			tex2;
	int						radius;
};
