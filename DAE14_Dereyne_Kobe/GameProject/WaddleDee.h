#pragma once
#include "Enemy.h"

class WaddleDee final : public Enemy
{
public:
	// Constructor & Destructor
	explicit WaddleDee(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~WaddleDee() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;

private:
};

