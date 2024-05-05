#pragma once
#include "Projectile.h"

class StarProjectile final : public Projectile
{
public:
	enum class Type
	{
		Single, Double
	};

	explicit StarProjectile(bool isFriendly = true);
	virtual ~StarProjectile() override = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;


	void SetType(StarProjectile::Type type);
private:

	StarProjectile::Type m_Type;
};

