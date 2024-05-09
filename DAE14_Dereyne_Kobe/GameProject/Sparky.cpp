#include "pch.h"
#include "Sparky.h"
#include "Spark.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Sparky", center, doesWorldCollsion)
	, m_ActionAccumSec{0}
	, m_AbilityDurationCounter{0}
	, m_BlinkCounter{0}
	, m_AboutToJump{false}
{
	m_Velocity = Vector2f(0.f, 0.f);
	m_pAbility = new Spark(false);
	m_AbilityType = AbilityType::Spark;


	const Vector2f m_VELOCITY_BIG_STAT		{ 0.f, 110.f };
	const Vector2f m_VELOCITY_SMALL_STAT	{ 0.f, m_VELOCITY_BIG_STAT.y / 2 };
	const Vector2f m_VELOCITY_BIG			{ 25.f, 110.f };
	const Vector2f m_VELOCITY_SMALL			{ m_VELOCITY_BIG.x, m_VELOCITY_BIG.y / 2 };
	m_vVelocities[0] = (m_VELOCITY_SMALL_STAT);
	m_vVelocities[1] = (m_VELOCITY_BIG_STAT);
	m_vVelocities[2] = (m_VELOCITY_SMALL);
	m_vVelocities[3] = (m_VELOCITY_BIG);
}

void Sparky::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);
	if (!IsEliminated())
	{
		UpdateAnimation();

		float xDistanceToKirby{ abs(m_Position.x - kirbyPos.x) };
		float yDistanceToKirby{ abs(m_Position.y - kirbyPos.y) };

		if (Collision::FloorCollision(this, world))
		{
			m_ActionAccumSec += elapsedSec;
			m_Velocity.x = 0;
			if (m_ActionAccumSec >= m_JUMP_CD - 0.15f and !m_pAbility->IsActive() and !m_AboutToJump)
			{
				m_AboutToJump = true;
			}
			
			if (m_ActionAccumSec >= m_JUMP_CD and !m_pAbility->IsActive())
			{
				m_AboutToJump = false;
				m_Direction = m_Position.x <= kirbyPos.x ? Direction::Right : Direction::Left;
				m_ActionAccumSec = 0;
				m_Velocity = m_vVelocities[rand() % m_vVelocities.size()];
				m_Position.y += 1;
			}

			if (m_CanMove and (xDistanceToKirby <= 40.f or (xDistanceToKirby <= 64.f and m_ActionAccumSec >= 3.f)) and yDistanceToKirby <= 64.f)
			{
				m_ActionAccumSec = 0;
				m_pAbility->Activate(m_Position, m_Direction);
				m_CanMove = false;
			}

		}

		m_AbilityDurationCounter += elapsedSec;
		if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
		{
			m_AbilityDurationCounter = 0;
			m_ActionAccumSec = 0;
			m_pAbility->Deactivate();
			m_CanMove = true;
		}
	}

	m_pAbility->Update(elapsedSec, world, this);
}

void Sparky::UpdateAnimation()
{
	if (!m_pAbility->IsActive())
	{
		m_CurrentFrameRow = 0;
		if (m_Velocity.y > 0.f) m_CurrentFrame = 0;
		if (abs(m_Velocity.y) < 0.001f) m_CurrentFrame = 1;
		if (m_AboutToJump) m_CurrentFrame = 2;
	}
	else
	{
		m_CurrentFrameRow = 1;
		if (m_AccumSec > 0.05f)
		{
			m_AccumSec = 0;
			m_CurrentFrame = (m_CurrentFrame + 1) % 2;
		}
	}
}
