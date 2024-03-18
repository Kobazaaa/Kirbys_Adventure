#include "pch.h"
#include "WaddleDee.h"

WaddleDee::WaddleDee(const Point2f& center)
	: Enemy("WaddleDee.png", center)
{
}

WaddleDee::~WaddleDee()
{
}

void WaddleDee::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
	WalkingAnimation();
}

void WaddleDee::WalkingAnimation()
{
	if (m_AccumSec >= 0.2f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
