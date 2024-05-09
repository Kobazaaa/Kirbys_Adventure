#include "pch.h"
#include "Fire.h"
#include "HotHead.h"

HotHead::HotHead(const Point2f& center, bool doesWorldCollsion)
	: Enemy("HotHead", center, doesWorldCollsion)
	, m_FireShot{ Projectile("Fire", Vector2f(60, 0), 2.f, false)}
	, m_ActionAccumSec{0}
	, m_AbilityDurationCounter{0}
	, m_BlinkCounter{0}
	, m_UsedFireShot{false}
{
	m_pAbility = new Fire(false);
	m_AbilityType = AbilityType::Fire;
}

void HotHead::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);
	
	if (!IsEliminated())
	{
		UpdateAnimation();

		m_ActionAccumSec += elapsedSec;
		if (m_ActionAccumSec >= 2.f)
		{
			if (m_CanMove) m_Direction = m_Position.x <= kirbyPos.x ? Direction::Right : Direction::Left;
			m_CanMove = false;
			m_CurrentFrameRow = 1;
			if (m_ActionAccumSec >= 2.f + 0.3f)
			{
				m_CurrentFrameRow = 2;
				if (utils::GetRandomBool() and !m_pAbility->IsActive() and !m_FireShot.IsActivated())
				{
					const Vector2f newVelocity
					{
						100.f,
						kirbyPos.y - m_Position.y
					};
					m_FireShot.SetVelocity(newVelocity);


					m_FireShot.Activate(m_Position, m_Direction);
					m_UsedFireShot = true;
					m_ActionAccumSec = 0;
				}
				else if (!m_pAbility->IsActive() and !m_FireShot.IsActivated())
				{
					m_pAbility->Activate(m_Position, m_Direction);
				}

				if (m_ActionAccumSec >= 2.f + 0.3f + 0.04f)
				{
					m_ActionAccumSec -= 0.04f;
					++m_BlinkCounter;
					m_CurrentFrame = m_BlinkCounter % 2;
				}
				if(m_pAbility->IsActive()) m_Position.x += m_BlinkCounter % 2 == 0 ? 0.5f : -0.5f;
			}
		}
		

		if (m_UsedFireShot and !m_FireShot.IsActivated())
		{
			m_UsedFireShot = false;
			m_CurrentFrameRow = 0;
			m_CurrentFrame = 0;
			m_CanMove = true;
			m_ActionAccumSec = 0;
		}
		else if (m_UsedFireShot)
		{
			if (m_ActionAccumSec >= 0.5f)
			{
				m_UsedFireShot = false;
				m_CurrentFrameRow = 0;
				m_CurrentFrame = 0;
				m_CanMove = true;
				m_ActionAccumSec = 0;
			}
		}

		if (m_pAbility->IsActive())
		{
			m_AbilityDurationCounter += elapsedSec;
			if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
			{
				m_CurrentFrameRow = 0;
				m_CurrentFrame = 0;
				m_CanMove = true;
				m_AbilityDurationCounter = 0;
				m_ActionAccumSec = 0;
				m_pAbility->Deactivate();
			}
		}
	}

	m_pAbility->Update(elapsedSec, world, this);
	m_FireShot.Update(elapsedSec, world);
}

void HotHead::Draw() const
{
	Enemy::Draw();
	m_FireShot.Draw();
}

void HotHead::UpdateAnimation()
{
	if (m_AccumSec >= m_WALK_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
