#include "pch.h"
#include "ViewFade.h"
#include "TextureManager.h"
#include "StateMachine.h"
#include "BaseState.h"
#include "PauseScreen.h"
#include "TitleScreen.h"
#include "GameOverScreen.h"
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
	, m_CurrentState	{  }
	, m_NextState		{  }
	, m_ViewPort		{ viewport }
{
	m_mStates[StateMachine::State::Titlescreen] = new TitleScreen(2, m_ViewPort);
	m_mStates[StateMachine::State::Gameplay] = new Gameplay(2, m_ViewPort);
	m_mStates[StateMachine::State::Pause] = new PauseScreen(2, m_ViewPort, dynamic_cast<Gameplay*>(m_mStates[StateMachine::State::Gameplay]));
	m_mStates[StateMachine::State::GameOver] = new GameOverScreen(2, m_ViewPort);

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

	delete m_mStates[StateMachine::State::GameOver];
	m_mStates[StateMachine::State::GameOver] = nullptr;
}

void StateMachine::Update(float elapsedSec)
{
	if (m_NextState != m_CurrentState and ViewFade::IsFadingIn())
	{
		m_Freeze = false;
		m_CurrentState = m_NextState;
	}
	if (m_mStates[m_CurrentState]) m_mStates[m_CurrentState]->Update(*this, elapsedSec, m_Freeze);
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

bool StateMachine::IsFrozen() const
{
	return m_Freeze;
}

StateMachine::State StateMachine::GetState() const
{
	return m_CurrentState;
}