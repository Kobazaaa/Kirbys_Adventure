#include "pch.h"
#include "Collision.h"
#include "Puff.h"

Puff::Puff(bool isFriendly)
	: Projectile("Puff", Vector2f(400.f, 0.f), 0.25f, isFriendly)
	, m_CanMove{true}
{
}

void Puff::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_CurrentAnimation = "Puff";
		m_AccumSec += elapsedSec;

		if (m_CanMove)
		{
			m_Velocity.x = m_START_SPEED * (1 - m_AccumSec / m_TravelTime);
			// MOVE
			m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		}
		else
		{
			m_Velocity.x = m_START_SPEED;
		}

		// RESET
		if (m_AccumSec >= m_TravelTime)
		{
			m_CanMove = false;
			if (m_AccumSec >= m_TravelTime + 0.3f)
			{
				m_CanMove = true;
				m_IsActive = false;
				m_AccumSec = 0;
			}
		}

		// COLLSION
		if (Collision::WallCollision(this, world))
		{
			m_CanMove = true;
			m_IsActive = false;
			m_AccumSec = 0;
		}

		// TODO remove this check, was only for while making the animations for ever entity
		if (m_pAnimationManager != nullptr) m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
}
