#include "pch.h"
#include "Fire.h"
#include "HotHead.h"

HotHead::HotHead(const Point2f& center, bool doesWorldCollsion)
	: Enemy("HotHead", center, doesWorldCollsion)
	, m_FireShot{ 2.f, false }
	, m_AbilityDurationCounter{0}
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
		if (m_AccumSec >= 2.f)
		{
			if (m_CanMove) m_Direction = m_Position.x <= kirbyPos.x ? Direction::Right : Direction::Left;
			m_CanMove = false;
			m_CurrentAnimation = "BlinkStart";
			if (m_AccumSec >= 2.f + 0.3f)
			{
				m_CurrentAnimation = "Blink";
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
					m_AccumSec = 0;
				}
				else if (!m_pAbility->IsActive() and !m_FireShot.IsActivated())
				{
					m_pAbility->Activate(m_Position, m_Direction);
				}

				if (m_pAbility->IsActive()) m_Position.x += m_pAnimationManager->GetCurrentFrame("Blink") % 2 == 0 ? 0.2f : -0.2f;
			}
		}
		
		if (m_CanMove) m_CurrentAnimation = "Walk";

		if (m_UsedFireShot)
		{
			if (!m_FireShot.IsActivated() or m_AccumSec >= 0.5f)
			{
				m_UsedFireShot = false;
				m_CurrentAnimation = "Walk";
				m_CanMove = true;
				m_AccumSec = 0;
			}
		}

		if (m_pAbility->IsActive())
		{
			m_AbilityDurationCounter += elapsedSec;
			if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
			{
				m_CurrentAnimation = "Walk";
				m_CanMove = true;
				m_AbilityDurationCounter = 0;
				m_AccumSec = 0;
				m_pAbility->Deactivate();
			}
		}
	}
	else
	{
		m_CurrentAnimation = "Walk";
		m_CanMove = true;
		m_AbilityDurationCounter = 0;
		m_AccumSec = 0;
		m_pAbility->Deactivate();
	}

	m_pAbility->Update(elapsedSec, world, this);
	m_FireShot.Update(elapsedSec, world);
}

void HotHead::Draw() const
{
	Enemy::Draw();
	m_FireShot.Draw();
}
