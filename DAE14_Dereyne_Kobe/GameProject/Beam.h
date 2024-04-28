#pragma once
#include "Ability.h"

class Beam final : public Ability
{
public:

	explicit Beam(bool isFriendly = false);
	virtual ~Beam() override = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	virtual void Activate(const Point2f& position, Projectile::Direction direction) override;

private:
	const int m_BEAM_SEGMENTS{ 5 };
	const float m_SPACING{5.f};

	float m_BeamAccumSec;

};

