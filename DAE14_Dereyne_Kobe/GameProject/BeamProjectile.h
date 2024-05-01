#pragma once
#include "Projectile.h"

class BeamProjectile final : public Projectile
{
public:
	explicit BeamProjectile(float travelTime, float diameter, bool isFriendly = false);
	~BeamProjectile() = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;

private:
	const float m_DIAMETER;
};

