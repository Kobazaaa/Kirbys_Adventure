#include "pch.h"
#include "StateMachine.h"

StateMachine::State StateMachine::m_CurrentState{ StateMachine::State::Gameplay };

void StateMachine::SetState(StateMachine::State newState)
{
	m_CurrentState = newState;
}

StateMachine::State StateMachine::GetState()
{
	return m_CurrentState;
}
