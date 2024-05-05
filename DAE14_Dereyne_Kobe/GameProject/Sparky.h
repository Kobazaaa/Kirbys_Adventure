#pragma once
#include "Enemy.h"

class Sparky final : public Enemy
{
public:
	// Constructor & Destructor
	explicit Sparky(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~Sparky() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;

private:
	float m_AbilityActivationTimer;

	float m_AbilityDurationCounter;
	const float m_ABILITY_DURATION{ 2.5f };
};