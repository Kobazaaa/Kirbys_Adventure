#pragma once
#include "BaseState.h"

class Texture;

class PauseScreen final : public BaseState
{
public:
	PauseScreen(float scale);

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:
	Texture* m_pTexture;
	float m_Scale;
};

