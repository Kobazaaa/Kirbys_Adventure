#include "pch.h"
#include "Beam.h"
#include "BeamProjectile.h"

Beam::Beam(bool isFriendly)
	: Ability(false)
	, m_BeamFlickerTimer{0}
{
	for (int index{}; index < m_BEAM_SEGMENTS; ++index)
	{
		m_vProjectiles.push_back(new BeamProjectile(0.5f, 2 * m_SPACING, isFriendly));
	}
}

void Beam::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_DURATION)
		{
			m_AccumSec = 0;
			Deactivate();
		}

		for (int index{}; index < m_BEAM_SEGMENTS; ++index)
		{
			m_vProjectiles[index]->Update(elapsedSec, world, index);

			m_vProjectiles[index]->SetStartPosition(owner->GetPosition());

			//if (!m_vProjectiles[index]->IsActivated())
			//{
			//	m_vProjectiles[index]->Activate(m_vProjectiles[index]->GetPosition(), owner->GetDirection());
			//	m_vProjectiles[index]->SetStartPosition(owner->GetPosition());
			//}
		}
		
		m_BeamFlickerTimer += elapsedSec;
		const float time{0.03f};
		if (m_BeamFlickerTimer > time)
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
			if (m_BeamFlickerTimer > time * 2)
			{
				m_BeamFlickerTimer = 0;
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
		m_BeamFlickerTimer = 0;
		Deactivate();
	}
}

void Beam::Activate(const Point2f& position, Direction direction)
{
	m_IsActive = true;
	m_AccumSec = 0;
	m_BeamFlickerTimer = 0;
	for (int index{}; index < m_BEAM_SEGMENTS; ++index)
	{
		m_vProjectiles[index]->Activate(position, direction);
	}
}
