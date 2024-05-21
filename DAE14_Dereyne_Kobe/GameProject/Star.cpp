#include "pch.h"
#include <iostream>
#include "Collision.h"
#include "Star.h"

Star::Star(Level* pLevel)
	: Projectile("StarProjectile", Vector2f(55, 0), 5.f, true, 16.f)
	, m_AbilityType{Entity::AbilityType::None}
{
	m_DoesEntityCollision = false;
}

void Star::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_CurrentAnimation = "Spin";

		m_AccumSec += elapsedSec;
		if (m_AccumSec >= m_TravelTime)
		{
			std::cout << "STAR DEACTIVATED\n";
			Deactivate();
		}

		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		m_Velocity.y += m_GRAVITY * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		if (Collision::WallCollision(this, world))	InvertDirection();

		m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
	}
	else
	{
		m_Velocity.y = 100;
	}
}

void Star::SetAbility(Entity::AbilityType ability)
{
	m_AbilityType = ability;
}

void Star::ApplyPlaySpace(Level* pLevel)
{
	const Rectf subLevel
	{
		0,
		pLevel->GetCurrentSubLevel() * pLevel->GetSubLevelHeight(),
		pLevel->GetWidth(),
		pLevel->GetSubLevelHeight()
	};

	if (!utils::IsRectInRect(GetHitBox(), subLevel))
	{
		if (GetHitBox().left < subLevel.left or
			GetHitBox().left + GetHitBox().width > subLevel.left + subLevel.width) InvertDirection();
		if (GetHitBox().bottom < subLevel.bottom or
			GetHitBox().bottom + GetHitBox().height > subLevel.bottom + subLevel.height) m_Velocity.y *= -1;
	}
	if (Collision::FloorCollision(this, pLevel->GetWorld())) m_Velocity.y = 175.f;

}

Entity::AbilityType Star::GetAbility() const
{
	return m_AbilityType;
}

void Star::InvertDirection()
{
	if (m_Direction == Direction::Left) m_Direction = Direction::Right;
	else m_Direction = Direction::Left;
}
