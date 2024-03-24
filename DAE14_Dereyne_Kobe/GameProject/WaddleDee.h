#pragma once
#include "Enemy.h"

class WaddleDee final : public Enemy
{
public:
	// Constructor & Destructor
	WaddleDee(const Point2f& center);
	virtual ~WaddleDee() override;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;

private:
	void WalkingAnimation();
};

