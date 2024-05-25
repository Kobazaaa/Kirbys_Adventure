#pragma once
#include "Projectile.h"

class StarProjectile final : public Projectile
{
public:
	enum class Type
	{
		Single, Double
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit StarProjectile(bool isFriendly = true);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;


	void SetType(StarProjectile::Type type);
private:
	// Private Variables
	StarProjectile::Type m_Type;
};

