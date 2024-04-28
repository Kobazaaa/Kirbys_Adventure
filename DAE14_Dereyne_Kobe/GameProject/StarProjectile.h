#pragma once
#include "Projectile.h"

class StarProjectile final : public Projectile
{
public:
	enum class Type
	{
		Single, Double
	};

	StarProjectile(bool isFriendly = true);
	~StarProjectile() = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;


	void SetType(StarProjectile::Type type);
private:

	float m_AnimationAccumSec;
	StarProjectile::Type m_Type;
};

