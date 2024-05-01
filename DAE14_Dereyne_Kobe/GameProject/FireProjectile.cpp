#include "pch.h"
#include "FireProjectile.h"
#include "Collision.h"

FireProjectile::FireProjectile(float travelTime, bool isFriendly)
	: Projectile("Fire", Vector2f(200, 0), travelTime, isFriendly)
{
}

void FireProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		if (m_AccumSec >= m_TravelTime)
		{
			m_AccumSec = 0;
			Deactivate();
		}
		else
		{
			m_AccumSec += elapsedSec;

			m_Position.x += static_cast<int>(m_Direction) * (m_Velocity.x * elapsedSec);
			m_Position.y += m_Velocity.y * elapsedSec;

			if (Collision::WallCollision(this, world)) Deactivate();
		}
	}
	else
	{
		Activate(m_StartPosition, m_Direction);
		m_Position.x = m_StartPosition.x + static_cast<int>(m_Direction) * 14.f;
		m_Velocity.y = (rand() % 101) - 50;
	}
}
