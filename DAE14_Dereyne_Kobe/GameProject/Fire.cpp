#include "pch.h"
#include "Fire.h"
#include "FireProjectile.h"

Fire::Fire(bool isFriendly)
	: Ability(2.5f)
{
	for (int index{}; index < m_FIREBALLS; ++index)
	{
		m_vProjectiles.push_back(new FireProjectile(m_PROJ_TRAVELTIME, isFriendly));
	}
}

void Fire::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_LifeTime)
		{
			m_AccumSec = 0;
			Deactivate();
		}


		for (int index{}; index < m_FIREBALLS; ++index)
		{
			if (m_AccumSec >= m_PROJ_TRAVELTIME / 2 * index)
			{
				m_vProjectiles[index]->Update(elapsedSec, world, index);
			}

			if (!m_vProjectiles[index]->IsActivated())
			{
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

void Fire::Activate(const Point2f& position, Direction direction)
{
	m_IsActive = true;
	m_AccumSec = 0;
	for (int index{}; index < m_FIREBALLS; ++index)
	{
		m_vProjectiles[index]->Activate(position, direction);
	}

}
