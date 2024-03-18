#include "pch.h"
#include "BrontoBurt.h"

BrontoBurt::BrontoBurt(const Point2f center)
	: Enemy("BrontoBurt.png", center)
	, m_SineTime{0}
	, m_NrCycles{0}
{
}

BrontoBurt::~BrontoBurt()
{
}

void BrontoBurt::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	m_SineTime += elapsedSec;

	Enemy::Update(elapsedSec, world);
	MovementAnimation();
	if (m_NrCycles < 6)
	{
		m_Velocity.y = 50 * sin(2 * M_PI / 2.f * m_SineTime);
		if (m_Velocity.y > -0.1f and m_Velocity.y < 0.1f) ++m_NrCycles;
	}
	else m_Velocity.y = 200.f;
}

void BrontoBurt::MovementAnimation()
{
	if ((m_Velocity.y < -20.f) and m_AccumSec >= 0.065f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame %= 2;
	}
}
