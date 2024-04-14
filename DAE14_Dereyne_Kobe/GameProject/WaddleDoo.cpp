#include "pch.h"
#include "WaddleDoo.h"

WaddleDoo::WaddleDoo(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Enemies/WaddleDoo.png", center, doesWorldCollsion)
{
}

void WaddleDoo::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);

	UpdateAnimation();
}

void WaddleDoo::UpdateAnimation()
{
	if (m_AccumSec >= m_WALK_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
