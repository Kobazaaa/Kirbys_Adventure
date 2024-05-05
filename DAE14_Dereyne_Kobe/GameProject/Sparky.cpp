#include "pch.h"
#include "Sparky.h"
#include "Spark.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Sparky", center, doesWorldCollsion)
	, m_AbilityActivationTimer{0}
	, m_AbilityDurationCounter{0}
{
	m_pAbility = new Spark(false);
	m_AbilityType = AbilityType::Spark;
}

void Sparky::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (!this->IsEliminated())
	{
		Enemy::Update(elapsedSec, world);

		m_AbilityActivationTimer += elapsedSec;
		if (m_AbilityActivationTimer >= 5.f)
		{
			m_AbilityActivationTimer = 0;
			m_pAbility->Activate(m_Position, m_Direction);
		}
	}
	m_pAbility->Update(elapsedSec, world, this);

	if (m_pAbility->IsActive())
	{
		m_AbilityDurationCounter += elapsedSec;
		if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
		{
			m_AbilityDurationCounter = 0;
			m_pAbility->Deactivate();
		}
	}
}
