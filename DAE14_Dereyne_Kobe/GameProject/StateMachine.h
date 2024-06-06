#pragma once
#include <map>
#include "BaseState.h"

class StateMachine final
{
public:
	enum class State
	{
		Titlescreen, Pause, Gameplay
	};

	explicit StateMachine();
	StateMachine(const StateMachine& other) = delete;
	StateMachine(StateMachine&& other) = delete;
	StateMachine& operator=(const StateMachine& rhs) = delete;
	StateMachine& operator=(StateMachine&& rhs) = delete;
	~StateMachine() noexcept;

	void Update(float elapsedSec);
	void Draw() const;

	void ChangeState(StateMachine::State newState);
	void Freeze();
	void Unfreeze();
	bool IsFrozen();

	StateMachine::State GetState();

private:
	std::map<StateMachine::State, BaseState*> m_mStates;
	State m_CurrentState;
	State m_NextState;
	bool m_Freeze;
};

