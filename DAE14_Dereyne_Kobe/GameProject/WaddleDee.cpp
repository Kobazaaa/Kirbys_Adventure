#include "pch.h"
#include "WaddleDee.h"

WaddleDee::WaddleDee(const Point2f& center)
	: Enemy("WaddleDee.png", center)
{
}

void WaddleDee::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
	UpdateAnimation();
}

#pragma region Animation
void WaddleDee::UpdateAnimation()
{
	if (m_AccumSec >= 0.2f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
#pragma endregion