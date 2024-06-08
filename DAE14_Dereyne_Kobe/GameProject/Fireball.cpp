#include "pch.h"
#include "Fireball.h"
#include "Collision.h"

Fireball::Fireball(float travelTime, bool isFriendly)
	: Projectile("Fire", Vector2f(0, 0), travelTime, isFriendly)
{
	m_DeactivateOnImpact = true;
}

void Fireball::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_CurrentAnimation = "Spin";

		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_TravelTime) Deactivate();
		if (Collision::WallCollision(this, world))	Deactivate();


		m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
}
