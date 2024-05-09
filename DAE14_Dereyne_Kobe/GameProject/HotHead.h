#pragma once
#include "Enemy.h"

class HotHead final : public Enemy
{
public:
	// Constructor & Destructor
	explicit HotHead(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~HotHead() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;
	virtual void Draw() const override;

private:
	//Animation
	void UpdateAnimation();

	// Variables
	Projectile m_FireShot;
	float m_ActionAccumSec;
	int m_BlinkCounter;
	bool m_UsedFireShot;

	float m_AbilityDurationCounter;
	const float m_ABILITY_DURATION{2.5f};
};