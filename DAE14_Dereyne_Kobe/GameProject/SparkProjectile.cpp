#include "pch.h"
#include "SparkProjectile.h"

SparkProjectile::SparkProjectile(float travelTime, bool isFriendly)
	: Projectile("Spark", Vector2f(0, 0), travelTime, isFriendly, 24.f)
{
}

void SparkProjectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
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

			m_Position.x += m_Velocity.x * elapsedSec;
			m_Position.y += m_Velocity.y * elapsedSec;

			if (m_IsFriendly)
			{
				m_CurrentAnimation = "Friendly";
			}
			else
			{
				m_CurrentAnimation = "Evil";
			}
		}

		m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
	else
	{
		Activate(m_StartPosition, m_Direction);
		const int speed{50};
		m_Velocity.x = utils::GetRandomInt(-1, 1) * utils::GetRandomInt(-speed, speed);
		m_Velocity.y = utils::GetRandomInt(-speed, speed);

		const int treshold{ 10 };
		if (m_Velocity.y >= -treshold and m_Velocity.y <= treshold)
		{
			m_Position.x = m_StartPosition.x + utils::GetSign(m_Velocity.x) * this->GetWidth();
		}
		else if (m_Velocity.x >= -treshold and m_Velocity.x <= treshold)
		{
			m_Position.y = m_StartPosition.y + utils::GetSign(m_Velocity.y) * this->GetHeight();
		}
		else
		{
			m_Position.x = m_StartPosition.x + utils::GetSign(m_Velocity.x) * this->GetWidth();
			m_Position.y = m_StartPosition.y + utils::GetSign(m_Velocity.y) * this->GetHeight();
		}
	}
}
