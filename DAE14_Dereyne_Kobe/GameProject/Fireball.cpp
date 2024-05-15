#include "pch.h"
#include "Fireball.h"
#include "Collision.h"

Fireball::Fireball(float travelTime, bool isFriendly)
	: Projectile("Fire", Vector2f(0, 0), travelTime, isFriendly)
{
}

void Fireball::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_CurrentAnimation = "Spin";

		m_AccumSec += elapsedSec;

		// MOVE
		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		// RESET
		if (m_AccumSec >= m_TravelTime)
		{
			Deactivate();
		}

		// COLLSION
		if (Collision::WallCollision(this, world))
		{
			Deactivate();
		}

		// TODO remove this check, was only for while making the animations for ever entity
		if (m_pAnimationManager != nullptr) m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
}
