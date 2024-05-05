#include "pch.h"
#include "Fire.h"
#include "HotHead.h"

HotHead::HotHead(const Point2f& center, bool doesWorldCollsion)
	: Enemy("HotHead", center, doesWorldCollsion)
	, m_FireShot{ Projectile("Fire", Vector2f(60, 0), 2.f, false)}
	, m_AbilityActivationTimer{0}
	, m_AbilityDurationCounter{0}
{
	m_pAbility = new Fire(false);
	m_AbilityType = AbilityType::Fire;
}

void HotHead::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (!this->IsEliminated())
	{
		Enemy::Update(elapsedSec, world);

		m_AbilityActivationTimer += elapsedSec;
		if (m_AbilityActivationTimer >= 5.f and !m_pAbility->IsActive() and !m_FireShot.IsActivated())
		{
			m_AbilityActivationTimer = 0;
			const int doAbility{ utils::GetRandomBool() };
			if (doAbility)
			{
				m_pAbility->Activate(m_Position, m_Direction);
			}
			else
			{
				m_FireShot.Activate(m_Position, m_Direction);
			}
		}
	}

	m_AbilityDurationCounter += elapsedSec;
	if (m_AbilityDurationCounter >= m_ABILITY_DURATION)
	{
		m_AbilityDurationCounter = 0;
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
