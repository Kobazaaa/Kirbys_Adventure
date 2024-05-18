#pragma once
#include "Enemy.h"

class PoppyBrosJr final : public Enemy
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit PoppyBrosJr(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~PoppyBrosJr() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;

private:
	// Privae Variabmes
	Direction m_StartDirection;
	float m_JumpAccumSec;

	//Private Constants
	const float m_JUMP_SPEED{ 55.f };
};

