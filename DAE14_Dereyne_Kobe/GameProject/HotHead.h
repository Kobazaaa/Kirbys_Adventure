#pragma once
#include "Enemy.h"

class HotHead final : public Enemy
{
public:
	// Constructor & Destructor
	explicit HotHead(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~HotHead() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	virtual void Draw() const override;

private:
	Projectile m_FireShot;
	float m_AccumSecAbility;

};