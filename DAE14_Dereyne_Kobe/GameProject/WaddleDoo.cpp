#include "pch.h"
#include "Beam.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center, bool doesWorldCollsion)
	: Enemy("WaddleDoo", center, doesWorldCollsion)
{
	m_pAbility = new Beam(false);
	m_AbilityType = AbilityType::Beam;

	m_Score = 300;
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
		if (m_AccumSec >= m_ABILITY_COOLDOWN)
		{
			// Random Jump
			if (utils::GetRandomBool() and m_CanMove)
			{
				m_Position.y += 1;
				m_Velocity.y = 150;
				m_AccumSec = 0;
				SoundManager::PlayEffect("EnemyJump");
			}
			// Else Use Ability
			else if (!m_pAbility->IsActive())
			{
				m_CanMove = false;
				m_CurrentAnimation = "Blink";
				m_Position.y += m_pAnimationManager->GetCurrentFrame("Blink") % 2 == 0 ? 0.5f : -0.5f;
				if (m_pAnimationManager->IsDone("Blink"))
				{
					m_CanMove = true;
					m_AccumSec = 0;
					m_pAbility->Activate(m_Position, m_Direction);
				}
			}
		}
		
		if (m_CanMove and !m_pAbility->IsActive())
		{
			m_CurrentAnimation = "Walk";
		}

		m_pAbility->Update(elapsedSec, world, this);
	}
	else
	{
		m_CurrentAnimation = "Walk";
		m_CanMove = true;
		//m_AccumSec = 0;
		m_pAbility->Deactivate();
	}
}
