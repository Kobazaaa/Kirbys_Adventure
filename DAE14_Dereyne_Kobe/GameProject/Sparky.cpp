#include "pch.h"
#include "Sparky.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Sparky", center, doesWorldCollsion)
{
}

void Sparky::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
}
