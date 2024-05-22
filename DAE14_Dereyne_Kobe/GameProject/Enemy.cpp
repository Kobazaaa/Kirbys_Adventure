#include "pch.h"
#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& textureName, const Point2f& center, int score, bool doesWorldCollsion)
	: Entity(textureName, 16, 16, center, score)
	, m_IsActivated			{ true }
	, m_IsEliminated		{ true }
	, m_SpawnPoint			{ center }
	, m_CanMove				{ true }
{
	m_Direction = Direction::Left;
	m_Velocity.x = m_WALK_SPEED;
}

void Enemy::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	if (!m_IsEliminated and m_IsActivated)
	{
		Entity::Update(elapsedSec, world);
		if (m_CanMove and (m_AbilityType == AbilityType::None or !m_pAbility->IsActive()))
		{
			m_Position.x += int(m_Direction) * elapsedSec * m_Velocity.x;
		}

		if (m_DoesWorldCollision)
		{
			m_IsGrounded = Collision::FloorCollision(this, world);
			if (Collision::WallCollision(this, world)) InverseDirection();
		}
	}
	else if (m_IsEliminated)
	{
		m_Direction = m_Position.x <= kirbyPos.x ? Direction::Right : Direction::Left;
	}
}

void Enemy::Draw() const
{
	if (!m_IsEliminated)
	{
		if (m_Direction == Direction::Right) Entity::Draw(true);
		else Entity::Draw(false);
	}
}

#pragma region Accessors
bool Enemy::IsActivated() const
{
	return m_IsActivated;
}
bool Enemy::IsEliminated() const
{
	return m_IsEliminated;
}
Rectf Enemy::GetSpawnRect() const
{
	const Rectf spawnRect
	{
		m_SpawnPoint.x - GetHitBox().width  / 2,
		m_SpawnPoint.y - GetHitBox().height / 2,
		GetHitBox().width, GetHitBox().height
	};
	return spawnRect;
}
Point2f Enemy::GetSpawnPoint() const
{
	return m_SpawnPoint;
}
#pragma endregion

#pragma region Mutators
void Enemy::IsActivated(bool isActivated)
{
	m_IsActivated = isActivated;
}
void Enemy::IsEliminated(bool isEliminated)
{
	m_IsEliminated = isEliminated;
}
void Enemy::Reset()
{
	IsEliminated(true);
	if (GetDirection() == Direction::Right) InverseDirection();
	m_AccumSec = 0;
	if(m_pAbility != nullptr) m_pAbility->Deactivate();
}
#pragma endregion