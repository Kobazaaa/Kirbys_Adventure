#pragma once
#include "Projectile.h"

class Fireball final : public Projectile
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Fireball(float travelTime, bool isFriendly = false);
	virtual ~Fireball() noexcept override = default;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;

private:
};

