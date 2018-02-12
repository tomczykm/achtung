#pragma once

enum EGameState {
	STATE_NULL,
	STATE_TEST,
	STATE_SETTINGS,
	STATE_EXIT
};

class CBaseGameState {
public:
	virtual void Logic() = 0;
	virtual void Render() = 0;
	virtual void Input() = 0;

	virtual ~CBaseGameState() {}
	static void SetNextState( EGameState newState );
	static void ChangeState();
};
