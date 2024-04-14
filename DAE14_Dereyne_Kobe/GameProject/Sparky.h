#pragma once
#include "Enemy.h"

class Sparky final : public Enemy
{
public:
	// Constructor & Destructor
	explicit Sparky(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~Sparky() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;

private:

};