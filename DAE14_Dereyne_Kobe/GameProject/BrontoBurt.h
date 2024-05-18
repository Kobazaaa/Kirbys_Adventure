#pragma once
#include "Enemy.h"

class BrontoBurt final : public Enemy
{
public:
	enum class Tactic
	{
		Wave, AscendingWave, Ascend, Straight
	};


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit BrontoBurt(const Point2f center, Tactic tactic);
	virtual ~BrontoBurt() override = default;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos) override;

	void Wave();
	void AscendingWave();
	void Ascend();
	void Straight(float elapsedSec, const Point2f& kirbyPos);

private:
	// Private Variables
	int m_NrCycles;
	float m_PreviousMultiplier{1.f};

	float m_FloatTime{0};
	bool m_IsFlying{false};

	Tactic m_Tactic;
};

