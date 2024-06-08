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
	explicit BaseState(Rectf viewport);
	virtual ~BaseState() = default;
	BaseState(const BaseState& other) = default;
	BaseState(BaseState&& other) = default;
	BaseState& operator=(const BaseState& rhs) = default;
	BaseState& operator=(BaseState&& rhs) = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(StateMachine& stateMachine, float elapsedSec, bool freeze) = 0;
	virtual void Draw() const = 0;

protected:
	const Rectf m_VIEWPORT;
};

