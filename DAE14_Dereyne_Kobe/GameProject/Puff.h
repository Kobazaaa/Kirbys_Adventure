#pragma once
#include "Projectile.h"

class Puff final : public Projectile
{
public:
	explicit Puff(bool isFriendly = true);
	~Puff() = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) override;

private:
	bool m_CanMove;
	const float m_START_SPEED{400.f};
};

