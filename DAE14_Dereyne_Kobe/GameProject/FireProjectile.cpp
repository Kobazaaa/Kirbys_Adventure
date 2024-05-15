#include "pch.h"
#include "FireProjectile.h"
#include "Collision.h"

FireProjectile::FireProjectile(float travelTime, bool isFriendly)
	: Projectile("Fire", Vector2f(150, 0), travelTime, isFriendly)
	, m_HorizontalFlip{false}
{
}

void FireProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		if (m_AccumSec >= m_TravelTime)
		{
			utils::GetRandomBool() ? m_CurrentAnimation = "One" : m_CurrentAnimation = "Two";
			m_HorizontalFlip = utils::GetRandomBool();
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

		// TODO remove this check, was only for while making the animations for ever entity
		if (m_pAnimationManager != nullptr) m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
	else
	{
		Activate(m_StartPosition, m_Direction);
		m_Position.x = m_StartPosition.x + static_cast<int>(m_Direction) * this->GetWidth();
		m_Velocity.y = static_cast<float>(utils::GetRandomInt(-50, 50));
	}
}

void FireProjectile::Draw() const
{
	glPushMatrix();
	{
		if (m_HorizontalFlip)
		{
			glTranslatef(0, m_Position.y, 0);
			glRotatef(-180, 1, 0, 0);
			glTranslatef(0, -m_Position.y, 0);
		}
		Projectile::Draw();
	}
	glPopMatrix();

}
