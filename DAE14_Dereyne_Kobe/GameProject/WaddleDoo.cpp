#include "pch.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Enemies/WaddleDoo.png", center, Ability::Type::Fire, doesWorldCollsion)
	, m_AccumSecAbility{0}
	, m_AccumSecJump{0}
{
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);

	UpdateAnimation();
	m_AccumSecAbility += elapsedSec;
	m_AccumSecJump += elapsedSec;

	if (!m_Ability.IsActivated())
	{
		if (m_AccumSecAbility >= 5)
		{
			m_AccumSecAbility = 0;
			m_Ability.Use();
		}

		if (m_AccumSecJump >= 3)
		{
			m_AccumSecJump = 0;
			int randIdx{ rand() % 2 };
			if (randIdx == 1)
			{
				m_Velocity.y = 150;
			}
		}
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
