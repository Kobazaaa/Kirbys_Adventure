#include "pch.h"
#include "WaddleDee.h"

WaddleDee::WaddleDee(const Point2f& center, bool doesWorldCollsion)
	: Enemy("WaddleDee", center, doesWorldCollsion)
{
}

void WaddleDee::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	if (!this->IsEliminated())
	{
		Enemy::Update(elapsedSec, world);
		UpdateAnimation();
	}
}

#pragma region Animation
void WaddleDee::UpdateAnimation()
{
	if (m_AccumSec >= m_WALK_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % 2;
	}
}
#pragma endregion