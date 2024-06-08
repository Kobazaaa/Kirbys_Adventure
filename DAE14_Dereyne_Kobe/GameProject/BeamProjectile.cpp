#include "pch.h"
#include <iostream>
#include "BeamProjectile.h"


BeamProjectile::BeamProjectile(float travelTime, float diameter, bool isFriendly)
	: Projectile("Beam", Vector2f(0, 0), travelTime, isFriendly)
	, m_DIAMETER{ diameter }
{
}

void BeamProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;
		m_CurrentAnimation = "Beam";
		if (m_AccumSec >= m_TravelTime)
		{
			m_AccumSec = 0;
			Deactivate();
		}
		else
		{
			const float angle{ 2 * float(M_PI) / 3};
			const float percentage{ m_AccumSec / m_TravelTime };
		
			m_Velocity.x = (index + 1) * m_DIAMETER * cosf( angle * percentage - float(M_PI) / 2);
			m_Velocity.y = (index + 1) * m_DIAMETER * sinf( angle * percentage - float(M_PI) / 2);

			m_Position.x = m_StartPosition.x + static_cast<int>(m_Direction) * m_Velocity.x;
			m_Position.y = m_StartPosition.y - m_Velocity.y;

		}

		m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
}
