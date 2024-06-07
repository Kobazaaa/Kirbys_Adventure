#pragma once
#include "BaseState.h"
#include "Animation.h"

class Texture;

class TitleScreen final : public BaseState
{
public:
	TitleScreen(float scale, Rectf viewport);

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float elapsedSec, bool freeze) override;
	virtual void Draw() const override;

private:
	Texture* m_pTexture;
	float m_Scale;

	Animation m_Animation;
};

