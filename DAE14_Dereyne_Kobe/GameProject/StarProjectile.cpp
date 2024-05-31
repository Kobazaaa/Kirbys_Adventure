#include "pch.h"
#include "Collision.h"
#include "StarProjectile.h"

StarProjectile::StarProjectile(bool isFriendly)
	: Projectile("StarProjectile", Vector2f(200.f, 0.f), 1.2f, isFriendly)
	, m_Type{ StarProjectile::Type::Single }
{
}

void StarProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_CurrentAnimation = "Spin";

		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;

		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_TravelTime)	Deactivate();

		if (Collision::WallCollision(this, world)) Deactivate();

		m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
}
