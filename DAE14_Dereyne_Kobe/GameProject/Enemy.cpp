#include "pch.h"
#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& filePathName, const Point2f& center)
	: Entity(filePathName, 16, 16, center)
	, m_IsActivated{true}
{
	m_Direction = Direction::Left;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (m_IsActivated)
	{
		Entity::Update(elapsedSec, world);
		m_Center.x += int(m_Direction) * elapsedSec * m_Velocity.x;
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

void Enemy::IsActivated(bool isActivated)
{
	m_IsActivated = false;
}
