#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(const std::string& filePathName, const Point2f& center)
	: Entity(filePathName, 16, 16, center)
{
	m_Direction = Direction::Left;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (m_Center.x - (m_Width * SCALE) / 2 <= 0) m_Direction = Direction::Right;
	if (m_Center.x + (m_Width * SCALE) / 2 >= 846.f) m_Direction = Direction::Left;

	Entity::Update(elapsedSec, world);
	WalkingAnimation();
	m_Center.x += int(m_Direction) * elapsedSec * m_Velocity.x;
}

void Enemy::Draw() const
{
	if (m_Direction == Direction::Right) Entity::Draw(true);
	else Entity::Draw(false);
}

void Enemy::WalkingAnimation()
{
	if (m_AccumSec >= 0.2f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
