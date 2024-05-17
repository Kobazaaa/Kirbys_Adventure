#pragma once
#include "Projectile.h"

class SparkProjectile final : public Projectile
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit SparkProjectile(float travelTime, bool isFriendly = false);
	virtual ~SparkProjectile() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;

private:

};
