#pragma once
#include "TextureManager.h"
#include "utils.h"
#include "ViewFade.h"
#include "SoundManager.h"
#include "StateMachine.h"

class StateMachine;
class Camera;

class BaseState
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit BaseState(Rectf viewport);
	BaseState(const BaseState& other) = default;
	BaseState(BaseState&& other) = default;
	BaseState& operator=(const BaseState& rhs) = default;
	BaseState& operator=(BaseState&& rhs) = default;
	virtual ~BaseState() = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(StateMachine& stateMachine, float elapsedSec, bool freeze) = 0;
	virtual void Draw() const = 0;

protected:
	// Private Datamembers
	const Rectf m_VIEWPORT;
};

