#pragma once
#include "Enemy.h"

class Sparky final : public Enemy
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Sparky(const Point2f& center, bool doesWorldCollsion = true);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;


private:
	// Private Variables
	std::vector<Vector2f> m_vVelocities{4};

	float m_AbilityDurationCounter;
	// Private Constants
	const float m_ABILITY_DURATION	{ 2.5f };
	const float m_JUMP_COOLDOWN		{ 0.5f };
};