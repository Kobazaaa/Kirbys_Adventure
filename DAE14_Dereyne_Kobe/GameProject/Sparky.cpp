#include "pch.h"
#include "Sparky.h"

Sparky::Sparky(const Point2f& center, bool doesWorldCollsion)
	: Enemy("Enemies/Sparky.png", center, Ability::Type::Spark, doesWorldCollsion)
{
}

void Sparky::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Enemy::Update(elapsedSec, world);
}
