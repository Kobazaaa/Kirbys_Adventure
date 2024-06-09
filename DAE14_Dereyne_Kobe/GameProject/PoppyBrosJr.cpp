#include "pch.h"
#include "PoppyBrosJr.h"

PoppyBrosJr::PoppyBrosJr(const Point2f& center)
	: Enemy("PoppyBrosJr", center, 200)
	, m_JumpAccumSec{ 0.f }
{
	m_StartDirection = m_Direction;
}

void PoppyBrosJr::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
		if (m_IsGrounded)
		{
			m_JumpAccumSec += elapsedSec;
			m_CurrentAnimation = "Grounded";
			m_Velocity.x = 0;

			if (m_JumpAccumSec >= 0.15f)
			{
				if (m_Direction == m_StartDirection)
				{
					if (rand() % 4 == 0)
					{
						InverseDirection();
					}
				}
				else
				{
					if (rand() % 4 > 0) InverseDirection();
				}


				m_JumpAccumSec = 0;
				m_Velocity.x = m_WALK_SPEED;
				m_Position.y += 1;
				m_Velocity.y = m_JUMP_SPEED;
				SoundManager::PlayEffect("EnemyJump");
			}
		}
		else
		{
			m_CurrentAnimation = "Air";
		}

	}
	else
	{
		m_StartDirection = m_Direction;
	}

}
