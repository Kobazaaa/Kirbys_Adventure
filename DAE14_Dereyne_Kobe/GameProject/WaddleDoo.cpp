#include "pch.h"
#include "Beam.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center, bool doesWorldCollsion)
	: Enemy("WaddleDoo", center, doesWorldCollsion)
	, m_ActionAccumSec{0}
	, m_BlinkCounter{0}
{
	m_pAbility = new Beam(false);
	m_AbilityType = AbilityType::Beam;
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
		UpdateAnimation();

		m_ActionAccumSec += elapsedSec;
		if (m_ActionAccumSec >= 2.3f)
		{
			if (utils::GetRandomBool() and m_CanMove)
			{
				m_Position.y += 1;
				m_Velocity.y = 150;
				m_ActionAccumSec = 0;
			}
			else if (!m_pAbility->IsActive())
			{
				m_CanMove = false;
				if (m_ActionAccumSec >= 2.3f + 0.06f)
				{
					m_ActionAccumSec -= 0.06f;
					++m_BlinkCounter;
					m_Position.y += m_BlinkCounter % 2 == 0 ? 0.7f : -0.7f;
					m_CurrentFrameRow = (m_CurrentFrameRow + 1) % 2;

					if (m_BlinkCounter >= 10)
					{
						m_CanMove = true;
						m_BlinkCounter = 0;
						m_CurrentFrameRow = 0;
						m_ActionAccumSec = 0;
						m_pAbility->Activate(m_Position, m_Direction);
					}
				}
			}
		}
		m_pAbility->Update(elapsedSec, world, this);
	}
}

void WaddleDoo::UpdateAnimation()
{
	if (m_AccumSec >= m_WALK_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
