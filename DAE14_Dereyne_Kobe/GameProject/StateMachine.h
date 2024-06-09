#pragma once
#include <map>

class BaseState;

class StateMachine final
{
public:
	enum class State
	{
		Titlescreen, Pause, Gameplay, GameOver
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit StateMachine(Rectf viewport);
	StateMachine(const StateMachine& other) = delete;
	StateMachine(StateMachine&& other) = delete;
	StateMachine& operator=(const StateMachine& rhs) = delete;
	StateMachine& operator=(StateMachine&& rhs) = delete;
	~StateMachine() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec);
	void Draw() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void ChangeState(StateMachine::State newState);
	void Freeze();
	void Unfreeze();


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsFrozen() const;
	StateMachine::State GetState() const;

private:
	// Private Datamambers
	std::map<StateMachine::State, BaseState*> m_mStates;
	State m_CurrentState;
	State m_NextState;

	Rectf m_ViewPort;
	bool m_Freeze;
};

