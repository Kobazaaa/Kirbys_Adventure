#include "pch.h"
#include "ViewFade.h"
#include "TextureManager.h"
#include "StateMachine.h"
#include "PauseScreen.h"
#include "TitleScreen.h"

void StateMachine::ChangeState(StateMachine::State newState)
{
	m_Freeze = true;

	if (m_mStates[m_CurrentState]) m_mStates[m_CurrentState]->Exit();

	m_NextState = newState;
	if (m_mStates[m_NextState]) m_mStates[m_NextState]->Enter();
}

StateMachine::StateMachine()
	: m_Freeze		{ false },
	m_CurrentState	{  },
	m_NextState		{  }
{
	m_mStates[StateMachine::State::Titlescreen] = new TitleScreen(2);
	m_mStates[StateMachine::State::Pause] = new PauseScreen(2);

	m_mStates[StateMachine::State::Titlescreen]->Enter();
}

StateMachine::~StateMachine() noexcept
{
	delete m_mStates[StateMachine::State::Pause];
	delete m_mStates[StateMachine::State::Titlescreen];
}

void StateMachine::Update(float elapsedSec)
{
	if (m_NextState != m_CurrentState and ViewFade::IsFadingIn())
	{
		m_Freeze = false;
		m_CurrentState = m_NextState;
	}
	if (m_mStates[m_CurrentState]) m_mStates[m_CurrentState]->Update(elapsedSec);
}

void StateMachine::Draw() const
{
	if (m_mStates.at(m_CurrentState)) m_mStates.at(m_CurrentState)->Draw();
}

void StateMachine::Freeze()
{
	m_Freeze = true;
}

void StateMachine::Unfreeze()
{
	m_Freeze = false;
}

bool StateMachine::IsFrozen()
{
	return m_Freeze;
}

StateMachine::State StateMachine::GetState()
{
	return m_CurrentState;
}
