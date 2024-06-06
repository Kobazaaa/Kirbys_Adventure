#pragma once
#include "TextureManager.h"
#include "utils.h"
#include "ViewFade.h"
#include "SoundManager.h"

class BaseState
{
public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

private:
};

