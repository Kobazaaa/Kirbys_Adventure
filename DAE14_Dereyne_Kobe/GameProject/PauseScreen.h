#pragma once
#include "BaseState.h"

class Texture;
class Gameplay;

class PauseScreen final : public BaseState
{
public:
	PauseScreen(float scale, Rectf viewport, Gameplay* gameplayState);

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(StateMachine& stateMachine, float elapsedSec, bool freeze) override;
	virtual void Draw() const override;

private:
	enum class Option{ Continue, Exit };
	
	Gameplay* m_pGameplay;

	Texture* m_pTexture;
	float m_Scale;
	Option m_Option;
};

