#pragma once
class StateMachine
{
public:
	enum class State
	{
		Titlescreen, Pause, Freeze, Gameplay
	};

	static void SetState(StateMachine::State newState);
	static StateMachine::State GetState();

private:
	static State m_CurrentState;
};

