#pragma once

enum class GameState {
	null,
	test,
	settings,
	quit
};

class BaseGameState {
public:
	virtual void Logic() = 0;
	virtual void Render() = 0;
	virtual void Input() = 0;
	virtual void PolledInput() = 0;

	virtual ~BaseGameState() {}
	static void SetNextState( GameState newState );
	static void ChangeState();
};
