#pragma once
#include "BaseState.h"

class Texture;
class Gameplay;

class GameOverScreen final : public BaseState
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit GameOverScreen(float scale, Rectf viewport);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(StateMachine& stateMachine, float elapsedSec, bool freeze) override;
	virtual void Draw() const override;

private:
	// Private Datamembers
	enum class Option { Continue, Exit };

	Texture* m_pTexture;
	float m_Scale;
	Option m_Option;

	Rectf m_SrcRect;

	float m_AccumSec;
	bool m_ShowOptions;
};

