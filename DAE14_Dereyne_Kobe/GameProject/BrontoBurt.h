#pragma once
#include "Enemy.h"

class BrontoBurt final : public Enemy
{
public:
	enum class Tactic
	{
		Wave, AscendingWave, Ascend, Straight
	};
	// Constructor & Destructor
	explicit BrontoBurt(const Point2f center, Tactic tactic, bool doesWorldCollsion = false);
	virtual ~BrontoBurt() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;

	void Wave();
	void AscendingWave();
	void Ascend();
	void Straight();

private:
	void UpdateAnimation();

	// Variables
	float m_SineTime;
	int m_NrCycles;

	Tactic m_Tactic;
};

