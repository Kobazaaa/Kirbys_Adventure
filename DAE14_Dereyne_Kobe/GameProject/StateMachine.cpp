#include "pch.h"
#include "ViewFade.h"
#include "TextureManager.h"
#include "StateMachine.h"
#include "PauseScreen.h"
#include "TitleScreen.h"
#include "Gameplay.h"

void StateMachine::ChangeState(StateMachine::State newState)
{
	m_Freeze = true;

	if (m_mStates[m_CurrentState]) m_mStates[m_CurrentState]->Exit();

	m_NextState = newState;
	if (m_mStates[m_NextState]) m_mStates[m_NextState]->Enter();
}

StateMachine::StateMachine(Rectf viewport)
	: m_Freeze			{ false }
	,  m_CurrentState	{  }
	, m_NextState		{  }
	, m_ViewPort		{ viewport }
{
	m_mStates[StateMachine::State::Titlescreen] = new TitleScreen(2, m_ViewPort);
	m_mStates[StateMachine::State::Pause] = new PauseScreen(2, m_ViewPort);
	m_mStates[StateMachine::State::Gameplay] = new Gameplay(2, m_ViewPort);

	m_mStates[StateMachine::State::Titlescreen]->Enter();
}

StateMachine::~StateMachine() noexcept
{
	delete m_mStates[StateMachine::State::Pause];
	m_mStates[StateMachine::State::Pause] = nullptr;

	delete m_mStates[StateMachine::State::Titlescreen];
	m_mStates[StateMachine::State::Titlescreen] = nullptr;

	delete m_mStates[StateMachine::State::Gameplay];
	m_mStates[StateMachine::State::Gameplay] = nullptr;
}

void StateMachine::Update(float elapsedSec)
{
	if (m_NextState != m_CurrentState and ViewFade::IsFadingIn())
	{
		m_Freeze = false;
		m_CurrentState = m_NextState;
	}
	if (m_mStates[m_CurrentState]) m_mStates[m_CurrentState]->Update(elapsedSec, m_Freeze);
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
