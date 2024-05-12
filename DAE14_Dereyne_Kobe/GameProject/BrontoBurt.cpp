#include "pch.h"
#include "BrontoBurt.h"

BrontoBurt::BrontoBurt(const Point2f center, Tactic tactic, bool doesWorldCollsion)
	: Enemy("BrontoBurt", center, doesWorldCollsion)
	, m_SineTime{0}
	, m_NrCycles{0}
	, m_Tactic{ tactic }
{
}

void BrontoBurt::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);
	if (!IsEliminated())
	{
		m_SineTime += elapsedSec;
		if (m_Tactic == Tactic::AscendingWave)
		{
			AscendingWave();
		}












		UpdateAnimation();
	}
	
}

void BrontoBurt::AscendingWave()
{
	if (m_NrCycles < 6)
	{
		m_Velocity.y = 50 * sinf(2 * M_PI / 2.f * m_SineTime);
		if (m_Velocity.y > -0.1f and m_Velocity.y < 0.1f) ++m_NrCycles;
	}
	else m_Velocity.y = 200.f;
}

#pragma region Animation
void BrontoBurt::UpdateAnimation()
{
	if ((m_Velocity.y < -20.f) and m_AccumSec >= 0.065f)
	{
		m_AccumSec = 0;
	//	++m_CurrentFrame %= 2;
	}
}
#pragma endregion