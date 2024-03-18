#pragma once
#include "Enemy.h"

class WaddleDee : public Enemy
{
public:
	WaddleDee(const Point2f& center);
	~WaddleDee();

	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

private:
	void WalkingAnimation();
};

