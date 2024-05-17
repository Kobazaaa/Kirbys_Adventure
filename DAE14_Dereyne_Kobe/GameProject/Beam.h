#pragma once
#include "Ability.h"

class Beam final : public Ability
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Beam(bool isFriendly = false);
	virtual ~Beam() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner) override;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Activate(const Point2f& position, Direction direction) override;

private:
	// Private Constants
	const int	m_BEAM_SEGMENTS	{ 5 };
	const float m_SPACING		{ 5.f };
	const float m_DURATION		{ 0.5f };

	// Private Variables
	float m_BeamFlickerTimer;
};

