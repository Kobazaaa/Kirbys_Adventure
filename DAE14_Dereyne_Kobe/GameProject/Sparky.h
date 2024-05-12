#pragma once
#include "Enemy.h"

class Sparky final : public Enemy
{
public:
	// Constructor & Destructor
	explicit Sparky(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~Sparky() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;

private:
	//Animation
	void UpdateAnimation();

	// Variables
	float m_ActionAccumSec;
	int m_BlinkCounter;
	bool m_CanJump;

	float m_AbilityDurationCounter;
	const float m_ABILITY_DURATION{ 2.5f };

	const float m_JUMP_CD{ 0.5f };
	std::vector<Vector2f> m_vVelocities{4};
};