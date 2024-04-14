#pragma once
#include "Enemy.h"

class BrontoBurt final : public Enemy
{
public:
	// Constructor & Destructor
	explicit BrontoBurt(const Point2f center, bool doesWorldCollsion = false);
	virtual ~BrontoBurt() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;

private:
	void UpdateAnimation();

	// Variables
	float m_SineTime;
	int m_NrCycles;
};

