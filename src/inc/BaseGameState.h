#pragma once

enum class GameState {
	null,
	test,
	settings,
	quit
};

class CBaseGameState {
public:
	virtual void Logic() = 0;
	virtual void Render() = 0;
	virtual void Input() = 0;
	virtual void PolledInput() = 0;

	virtual ~CBaseGameState() {}
	static void SetNextState( GameState newState );
	static void ChangeState();
};
