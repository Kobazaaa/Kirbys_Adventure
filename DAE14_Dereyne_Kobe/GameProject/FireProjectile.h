#pragma once
#include "Projectile.h"

class FireProjectile final : public Projectile
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit FireProjectile(float travelTime, bool isFriendly = false);
	virtual ~FireProjectile() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;
	virtual void Draw() const override;

private:
	// Private Variables
	bool m_HorizontalFlip;
};

