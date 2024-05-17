#pragma once
#include "Ability.h"

class Fire final : public Ability
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Fire(bool isFriendly = false);
	virtual ~Fire() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner) override;
	virtual void Activate(const Point2f& position, Direction direction) override;


private:
	// Private Constants
	const int m_FIREBALLS{2};
	const float m_PROJ_TRAVELTIME{0.2f};
};

