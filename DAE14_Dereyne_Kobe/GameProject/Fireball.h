#pragma once
#include "Projectile.h"

class Fireball final : public Projectile
{
public:
	explicit Fireball(float travelTime, bool isFriendly = false);
	virtual ~Fireball() override = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;

private:
};

