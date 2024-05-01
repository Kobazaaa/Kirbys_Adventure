#include "pch.h"
#include "Fire.h"
#include "HotHead.h"

HotHead::HotHead(const Point2f& center, bool doesWorldCollsion)
	: Enemy("HotHead", center, doesWorldCollsion)
	, m_FireShot{ Projectile("Fire", Vector2f(60, 0), 2.f, false)}
	, m_AccumSecAbility{0}
{
	m_pAbility = new Fire(false);
}

void HotHead::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);

	m_pAbility->Update(elapsedSec, world, this);
	m_FireShot.Update(elapsedSec, world);

	m_AccumSecAbility += elapsedSec;
	if (m_AccumSecAbility >= 5.f and !m_pAbility->IsActive() and !m_FireShot.IsActivated())
	{
		m_AccumSecAbility = 0;
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

void HotHead::Draw() const
{
	Enemy::Draw();
	m_FireShot.Draw();
}
