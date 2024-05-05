#pragma once
#include "Projectile.h"

class FireProjectile final : public Projectile
{
public:
	explicit FireProjectile(float travelTime, bool isFriendly = false);
	virtual ~FireProjectile() override = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;
	virtual void Draw() const override;

private:
	bool m_HorizontalFlip;

};

