#include "pch.h"
#include "HotHead.h"

HotHead::HotHead(const Point2f& center, bool doesWorldCollsion)
	: Enemy("HotHead", center, doesWorldCollsion)
{
}

void HotHead::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
}
