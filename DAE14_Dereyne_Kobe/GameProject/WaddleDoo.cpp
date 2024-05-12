#include "pch.h"
#include "Beam.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center, bool doesWorldCollsion)
	: Enemy("WaddleDoo", center, doesWorldCollsion)
	, m_ActionAccumSec{0}
{
	m_pAbility = new Beam(false);
	m_AbilityType = AbilityType::Beam;

	m_pAnimationManager->LoadFromFile("Enemies/WaddleDoo.xml");
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
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
				m_CurrentAnimation = "Blink";
				m_Position.y += m_pAnimationManager->GetCurrentFrame("Blink") % 2 == 0 ? 0.5f : -0.5f;
				if (m_pAnimationManager->IsDone("Blink"))
				{
					m_CanMove = true;
					m_ActionAccumSec = 0;
					m_pAbility->Activate(m_Position, m_Direction);
				}
			}
		}
		else m_CurrentAnimation = "Walk";
		m_pAbility->Update(elapsedSec, world, this);
	}
}