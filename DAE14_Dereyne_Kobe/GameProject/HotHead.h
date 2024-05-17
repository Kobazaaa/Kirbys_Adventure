#pragma once
#include "Enemy.h"
#include "Fireball.h"

class HotHead final : public Enemy
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit HotHead(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~HotHead() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;
	virtual void Draw() const override;


private:
	// Private Variables
	Fireball m_FireShot;
	bool m_UsedFireShot;

	float m_AbilityDurationCounter;
	// Private Constants
	const float m_ABILITY_DURATION{ 2.5f };
	const float m_ABILITY_COOLDOWN{ 2.f };
};