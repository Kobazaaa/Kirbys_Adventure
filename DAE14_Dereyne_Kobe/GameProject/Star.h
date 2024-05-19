#pragma once
#include "Projectile.h"
#include "Level.h"
#include "Entity.h"

class Star final : public Projectile
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Star(Level* pLevel);
	virtual ~Star() noexcept override = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0);
	virtual void SetAbility(Entity::AbilityType ability);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual Entity::AbilityType GetAbility() const;

private:
	// Private Update Functions
	void InvertDirection();

	// Private Variables
	Level* m_pLevel;
	Entity::AbilityType m_AbilityType;

	// Private Constants
	const float m_GRAVITY{ -300.f };
};

