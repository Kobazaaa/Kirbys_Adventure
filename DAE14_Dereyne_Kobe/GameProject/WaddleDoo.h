#pragma once
#include "Enemy.h"

class WaddleDoo final : public Enemy
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit WaddleDoo(const Point2f& center, bool doesWorldCollsion = true);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;


private:
	const float m_ABILITY_COOLDOWN	{ 2.3f };
};

