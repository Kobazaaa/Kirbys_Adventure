#pragma once
#include "Enemy.h"

class BrontoBurt : public Enemy
{
public:
	BrontoBurt(const Point2f center);
	~BrontoBurt();

	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);


private:
	void MovementAnimation();

	float m_SineTime;
	int m_NrCycles;
};

