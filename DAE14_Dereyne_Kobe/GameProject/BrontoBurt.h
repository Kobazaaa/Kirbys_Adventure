#pragma once
#include "Enemy.h"

class BrontoBurt final : public Enemy
{
public:
	// Constructor & Destructor
	BrontoBurt(const Point2f center);
	virtual ~BrontoBurt() override;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;

private:
	void MovementAnimation();

	// Variables
	float m_SineTime;
	int m_NrCycles;
};

