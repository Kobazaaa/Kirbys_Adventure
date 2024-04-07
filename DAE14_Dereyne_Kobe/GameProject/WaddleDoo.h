#pragma once
#include "Enemy.h"

class WaddleDoo final : public Enemy
{
public:
	// Constructor & Destructor
	WaddleDoo(const Point2f& center);
	virtual ~WaddleDoo() override = default;

	// Behavioural
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
private:
	//Animation
	void UpdateAnimation();

	//
};

