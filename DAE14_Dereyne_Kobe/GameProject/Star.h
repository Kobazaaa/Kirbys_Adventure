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


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0);
	void SetAbility(Entity::AbilityType ability);
	void ApplyPlaySpace(Level* pLevel);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Entity::AbilityType GetAbility() const;

private:
	// Private Update Functions
	void InvertDirection();

	// Private Variables
	Entity::AbilityType m_AbilityType;

	// Private Constants
	const float m_GRAVITY{ -300.f };
};

