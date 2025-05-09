#include "pch.h"
#include "Spark.h"
#include "SparkProjectile.h"

Spark::Spark(bool isFriendly)
	: Ability(true)
{
	for (int index{}; index < m_SPARKS; ++index)
	{
		m_vProjectiles.push_back(new SparkProjectile(m_PROJ_TRAVELTIME, isFriendly));
	}
}

void Spark::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;
		for (int index{}; index < m_SPARKS; ++index)
		{
			if (m_AccumSec >= m_PROJ_TRAVELTIME / 2 * index)
			{
				m_vProjectiles[index]->Update(elapsedSec, world, index);
			}

			if (!m_vProjectiles[index]->IsActivated())
			{
				SoundManager::PlayEffect("Spark");
				m_vProjectiles[index]->SetStartPosition(owner->GetPosition());
			}
		}

	}
	else
	{
		m_AccumSec = 0;
		Deactivate();
	}
}

void Spark::Activate(const Point2f& position, Direction direction)
{
	m_IsActive = true;
	m_AccumSec = 0;
	for (int index{}; index < m_SPARKS; ++index)
	{
		m_vProjectiles[index]->Activate(position, direction);
	}
}
