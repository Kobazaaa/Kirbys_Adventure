#pragma once
#include <vector>
#include "Entity.h"
#include "Vector2f.h"
#include "Texture.h"
#include "Puff.h"
#include "StarProjectile.h"
#include "Beam.h"
#include "Fire.h"
#include "Spark.h"

class Enemy;
class Level;

class Kirby final : public Entity
{
public:
	enum class State
	{
		None, Walk, Slide, Jump, Falling, Flight, Inhaling, Exhaling, Swallow, Ability, EnterDoor
	};
	enum class Card
	{
		Ability, Nothing, Damaged
	};


	// Constructor & Destructor
	explicit Kirby(const Point2f& center, Level* const level);
	virtual ~Kirby() override = default;
	friend std::ostream& operator<<(std::ostream& out, const Kirby& kirby);
	std::string EnumToString(Kirby::State state) const;

	// Behavioral
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	void Draw() const;
	void HitEnemy(const Point2f& enemyPos);
	void InhaledEnemy(Enemy* enemy);
	bool DoDoorChecks();

	void Reset();

	// Accessors
	Kirby::State GetCurrentState()  const;
	Rectf		 GetInhaleRect()	const;
	int			 GetHealth()		const;
	int			 GetLives()			const;
	int			 GetScore()			const;
	Kirby::Card  GetCard()			const;

	// Mutators

private:
	// Private Update Functions
	void UpdateSprite();
	void MovementUpdate(float elapsedSec);
	void ApplyPlaySpace();

	// Animation Functions
	void AnimateWalk();

	// Functions that check whether or not Kirby can perform a certain action in his current state
	bool CanMoveWithCurrentState()		const;
	bool CanJumpWithCurrentState()		const;
	bool CanSlideWithCurrentState()		const;
	bool CanFlyWithCurrentState()		const;
	bool CanInhaleWithCurrentState()	const;

	// Timers
	const float m_INVINCIBILITY_TIME	{3.f};
	const float m_SLIDING_TIME			{0.5f};

	// Private Data Members
	State		m_CurrentState{State::None};
	State		m_OldState{State::None};

	const float		m_JUMP_SPEED	{ 20.f - m_GRAVITY / 2.f};
	const float		m_WALK_SPEED	{ 60.f };
	const float		m_RUN_SPEED		{ 1.5f * m_WALK_SPEED };
	const float		m_SLIDE_SPEED	{ 2 * m_WALK_SPEED };
	const float		m_FLIGHT_SPEED	{ 50.f };
	
	int			m_Health;
	int			m_Lives;
	int			m_Score;

	float m_InvincibleAccumSec{};
	float m_AbilityActivationAccumSec{};

	bool m_InhaledEnemy;
	Enemy* m_pInhaledEnemy;

	Puff m_Puff;
	StarProjectile m_StarProj;

	Card m_Card;

	bool m_IsSliding{false};
	bool m_IsRunning{false};
	bool m_IsTurning{false};

	Level* const m_pLevel;
};

