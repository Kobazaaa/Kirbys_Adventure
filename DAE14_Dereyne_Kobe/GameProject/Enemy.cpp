#include "pch.h"
#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& textureName, const Point2f& center, bool doesWorldCollsion)
	: Entity(textureName, 16, 16, center)
	, m_IsActivated{true}
	, m_IsEliminated{true}
	, m_SpawnPoint{center}
	, m_DoesWorldCollision{ doesWorldCollsion }
{
	m_Direction = Direction::Left;
}

void Enemy::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (!m_IsEliminated and m_IsActivated)
	{
		if (m_pAbility != nullptr and !m_pAbility->IsActive())
		{
			Entity::Update(elapsedSec, world);
			m_Position.x += int(m_Direction) * elapsedSec * m_Velocity.x;
		}
		else if (m_pAbility == nullptr)
		{
			Entity::Update(elapsedSec, world);
			m_Position.x += int(m_Direction) * elapsedSec * m_Velocity.x;
		}
	}

	if (m_DoesWorldCollision)
	{
		m_Velocity.x = m_WALK_SPEED;
		Collision::FloorCollision(this, world);
		if (Collision::WallCollision(this, world)) InverseDirection();
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
}
#pragma endregion