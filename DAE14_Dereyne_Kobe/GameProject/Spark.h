#pragma once
#include "Ability.h"

class Spark final : public Ability
{
public:
	explicit Spark(bool isFriendly = false);
	virtual ~Spark() override = default;

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner) override;
	virtual void Activate(const Point2f& position, Direction direction) override;

private:
	const int m_SPARKS{ 2 };
	const float m_PROJ_TRAVELTIME{ 0.13f };

};

