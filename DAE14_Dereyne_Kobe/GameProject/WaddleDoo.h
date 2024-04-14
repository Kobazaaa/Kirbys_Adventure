#pragma once
#include "Enemy.h"

class WaddleDoo final : public Enemy
{
public:
	// Constructor & Destructor
	explicit WaddleDoo(const Point2f& center, bool doesWorldCollsion = true);
	virtual ~WaddleDoo() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
private:
	//Animation
	void UpdateAnimation();

	//
};

