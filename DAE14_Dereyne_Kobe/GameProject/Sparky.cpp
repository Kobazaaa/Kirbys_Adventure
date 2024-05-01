#include "pch.h"
#include "Sparky.h"
#include "Spark.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Sparky", center, doesWorldCollsion)
	, m_AccumSecAbility{0}
{
	m_pAbility = new Spark(false);
}

void Sparky::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
	m_pAbility->Update(elapsedSec, world, this);

	m_AccumSecAbility += elapsedSec;
	if (m_AccumSecAbility >= 5.f)
	{
		m_AccumSecAbility = 0;
		m_pAbility->Activate(m_Position, m_Direction);
	}
}
