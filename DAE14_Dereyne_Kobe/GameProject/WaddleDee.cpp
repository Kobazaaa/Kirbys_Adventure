#include "pch.h"
#include "WaddleDee.h"

WaddleDee::WaddleDee(const Point2f& center)
	: Enemy("WaddleDee", center, 200)
{
}

void WaddleDee::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
		m_CurrentAnimation = "Walk";
	}
}