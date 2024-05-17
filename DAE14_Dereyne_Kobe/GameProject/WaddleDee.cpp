#include "pch.h"
#include "WaddleDee.h"

WaddleDee::WaddleDee(const Point2f& center, bool doesWorldCollsion)
	: Enemy("WaddleDee", center, doesWorldCollsion)
{
	m_Score = 200;
}

void WaddleDee::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos)
{
	Enemy::Update(elapsedSec, world, kirbyPos);

	if (!IsEliminated())
	{
		m_CurrentAnimation = "Walk";
	}
}