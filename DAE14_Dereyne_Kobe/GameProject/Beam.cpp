#include "pch.h"
#include "Beam.h"
#include "BeamProjectile.h"

Beam::Beam(bool isFriendly)
	: Ability( 0.5f )
	, m_BeamAccumSec{0}
{
	for (int index{}; index < m_BEAM_SEGMENTS; ++index)
	{
		m_vProjectiles.push_back(new BeamProjectile(m_LifeTime, 2 * m_SPACING, isFriendly));
	}
}

void Beam::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_LifeTime)
		{
			m_AccumSec = 0;
			Deactivate();
		}
		
		
		for (int index{}; index < m_BEAM_SEGMENTS; ++index)
		{
			m_vProjectiles[index]->Update(elapsedSec, world, index);

			m_vProjectiles[index]->SetStartPosition(owner->GetPosition());
		}
		
		m_BeamAccumSec += elapsedSec;
		const float time{0.03f};
		if (m_BeamAccumSec > time)
		{
			for (int index{}; index < m_BEAM_SEGMENTS; ++index)
			{
				if (index % 2 == 0)
				{
					m_vProjectiles[index]->Hide();
				}
				else
				{
					m_vProjectiles[index]->Reveal();
				}
			}
			if (m_BeamAccumSec > time * 2)
			{
				m_BeamAccumSec = 0;
				for (int index{}; index < m_BEAM_SEGMENTS; ++index)
				{
					if (index % 2 == 0)
					{
						m_vProjectiles[index]->Reveal();
					}
					else
					{
						m_vProjectiles[index]->Hide();
					}
				}

			}
		}
	}
	else
	{
		m_AccumSec = 0;
		m_BeamAccumSec = 0;
		Deactivate();
	}
}

void Beam::Activate(const Point2f& position, Direction direction)
{
	m_IsActive = true;
	m_AccumSec = 0;
	m_BeamAccumSec = 0;
	for (int index{}; index < m_BEAM_SEGMENTS; ++index)
	{
		m_vProjectiles[index]->Activate(position, direction);
	}
}
