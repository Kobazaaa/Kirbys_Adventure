#include "pch.h"
#include "Sparky.h"
#include "Spark.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Sparky", center, doesWorldCollsion)
	, m_AbilityDurationCounter{0}
{
	m_Velocity.x = 0.f;
	m_pAbility = new Spark(false);
	m_AbilityType = AbilityType::Spark;

	m_Score = 300;

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
		float xDistanceToKirby{ abs(m_Position.x - kirbyPos.x) };
		float yDistanceToKirby{ abs(m_Position.y - kirbyPos.y) };

		if (Collision::FloorCollision(this, world))
		{
			m_Velocity.x = 0;
			m_CurrentAnimation = "Idle";

			if (m_AccumSec >= m_JUMP_COOLDOWN and !m_pAbility->IsActive())
			{
				if (m_pAnimationManager->IsDone("Idle"))
				{
					m_AccumSec = 0;
					m_Direction = m_Position.x <= kirbyPos.x ? Direction::Right : Direction::Left;
					m_Velocity = m_vVelocities[rand() % m_vVelocities.size()];
					m_Position.y += 1;
				}
			}

			if (m_CanMove and (xDistanceToKirby <= 40.f or (xDistanceToKirby <= 128.f and m_AccumSec >= 3.f)) and yDistanceToKirby <= 64.f)
			{
				m_pAbility->Activate(m_Position, m_Direction);
				m_CanMove = false;
			}
		}
		else m_CurrentAnimation = "Jump";


		m_AbilityDurationCounter += elapsedSec;
		if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
		{
			m_AbilityDurationCounter = 0;
			m_AccumSec = 0;
			m_pAbility->Deactivate();
			m_CanMove = true;
		}
		if (m_pAbility->IsActive()) m_CurrentAnimation = "Ability";
	}

	m_pAbility->Update(elapsedSec, world, this);
}