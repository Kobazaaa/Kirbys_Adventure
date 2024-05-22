#include "pch.h"
#include "BrontoBurt.h"

BrontoBurt::BrontoBurt(const Point2f center, Tactic tactic)
	: Enemy("BrontoBurt", center, 300)
	, m_NrCycles{0}
	, m_Tactic{ tactic }
{
}

void BrontoBurt::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);
	if (!IsEliminated())
	{
		switch (m_Tactic)
		{
		case BrontoBurt::Tactic::Wave:
			Wave();
			break;
		case BrontoBurt::Tactic::AscendingWave:
			AscendingWave();
			break;
		case BrontoBurt::Tactic::Ascend:
			Ascend();
			break;
		case BrontoBurt::Tactic::Straight:
			Straight(elapsedSec, kirbyPos);
			break;
		default:
			break;
		}
	}
	else
	{
		m_IsFlying = false;
		m_AccumSec = 0;
		m_NrCycles = 0;
		m_FloatTime = 0;
		m_GravityMultiplier = 1.f;
		m_Velocity.y = 0.f;
	}
	
}

void BrontoBurt::Wave()
{
	m_DoesWorldCollision = false;
	if (m_Velocity.y <= m_GRAVITY * m_GravityMultiplier / 3) m_GravityMultiplier = -1.f;
	else m_GravityMultiplier = 1.f;

	if (m_GravityMultiplier < 0.f) m_CurrentAnimation = "FastFlap";
	else m_CurrentAnimation = "NoFlap";

}
void BrontoBurt::AscendingWave()
{
	m_DoesWorldCollision = false;
	if (m_NrCycles < 6)
	{
		if (m_Velocity.y <= m_GRAVITY * m_GravityMultiplier / 3) m_GravityMultiplier = -1.f;
		else m_GravityMultiplier = 1.f;
	
		if (m_PreviousMultiplier != m_GravityMultiplier)
		{
			++m_NrCycles;
			m_PreviousMultiplier = m_GravityMultiplier;
		}

		if (m_GravityMultiplier < 0.f) m_CurrentAnimation = "FastFlap";
		else m_CurrentAnimation = "NoFlap";


	}
	else
	{
		m_Velocity.y = 200.f;
		m_CurrentAnimation = "FastFlap";
	}
}
void BrontoBurt::Ascend()
{
	if (abs(m_Velocity.y) < 0.001f)
	{
		m_Position.y += utils::GetSign(m_HitInfo.normal.y) * 1.f;
		m_AccumSec = 0;
	}

	m_DoesWorldCollision = true;
	if (m_AccumSec >= 1.f)
	{
		m_DoesWorldCollision = false;
	}
	m_Velocity.x = m_WALK_SPEED;
	m_Velocity.y = utils::GetSign(m_HitInfo.normal.y) * m_WALK_SPEED;

	m_CurrentAnimation = "SlowFlap";
}
void BrontoBurt::Straight(float elapsedSec, const Point2f& kirbyPos)
{
	m_Velocity.x = 0;
	m_DoesWorldCollision = true;
	if (abs(m_Position.x - kirbyPos.x) < 48.f and !m_IsFlying)
	{
		m_Position.y += 1;
		m_IsFlying = true;
		m_Velocity.y = 150.f;
		m_CurrentAnimation = "SlowFlap";
	}
	else if (!m_IsFlying)
	{
		m_DoesWorldCollision = true;
		m_CurrentAnimation = "NoFlap";
	}

	if (m_Velocity.y < 0.f) m_Direction = static_cast<Direction>(utils::GetSign(kirbyPos.x - m_Position.x));

	if (m_IsFlying)
	{
		m_FloatTime += elapsedSec;

		if (m_FloatTime >= 0.8f)
		{
			m_DoesWorldCollision = false;
			m_Velocity.y = 0.f;
			m_Velocity.x = 250.f;
			m_GravityMultiplier = 0.f;
			m_CurrentAnimation = "FastFlap";
		}
	}
	else
	{
		m_GravityMultiplier = 1.f;
		m_DoesWorldCollision = true;
		m_FloatTime = 0;
		m_CurrentAnimation = "NoFlap";
	}
}

BrontoBurt::Tactic BrontoBurt::GetTactic() const
{
	return m_Tactic;
}
