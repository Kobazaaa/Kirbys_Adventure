#pragma once
#include <vector>
#include "Entity.h"
#include "Vector2f.h"
#include "Texture.h"
#include "Puff.h"
#include "StarProjectile.h"
#include "Star.h"
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
		None, Walk, Slide, Jump, Falling, Flight, Inhaling, Exhaling, Swallow, Ability, EnterDoor, Hit, Land, Dead
	};
	enum class Card
	{
		Ability, Nothing, Damaged
	};


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Kirby(const Point2f& center, Level* const level);
	virtual ~Kirby() override = default;
	friend std::ostream& operator<<(std::ostream& out, const Kirby& kirby);
	std::string EnumToString(Kirby::State state) const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	void Draw() const;
	void HitEnemy(const Point2f& enemyPos);
	void InhaledEnemy(AbilityType ability);
	bool DoDoorChecks(bool setPos);

	void Reset();


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Kirby::State GetCurrentState()  const;
	Rectf		 GetInhaleRect()	const;
	int			 GetHealth()		const;
	int			 GetLives()			const;
	Kirby::Card  GetCard()			const;
	bool		 IsInvincible()		const;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AddToScore(int addition);

private:

	// float elapsedSec, const std::vector<std::vector<Point2f>>& world
	// Private Update Functions
	void AbilityUpdate(float elapsedSec, const std::vector<std::vector<Point2f>>& world);
	void Death();
	void Invincibility(float elapsedSec);
	void UserInput();
	void MovementUpdate(float elapsedSec);
	void MechanicUpdate(float elapsedSec);

	void Collisions(const std::vector<std::vector<Point2f>>& world);

	void SlopeHandling();
	void ApplyFriction(float elapsedSec);

	void Animate();
	void ApplyPlaySpace();

	// Functions that check whether or not Kirby can perform a certain action in his current state
	bool CanMoveWithCurrentState()		const;
	bool CanJumpWithCurrentState()		const;
	bool CanSlideWithCurrentState()		const;
	bool CanFlyWithCurrentState()		const;
	bool CanInhaleWithCurrentState()	const;

	// Timers (constants)
	const float m_INVINCIBILITY_TIME	{3.f};
	const float m_SLIDING_TIME			{0.5f};

	// States
	State		m_CurrentState	{State::None};
	State		m_OldState		{State::None};
	Card		m_Card;

	bool		m_HasInhaledAbility;
	AbilityType m_InhaledAbilityType;

	// Kirby Statistics
	int			m_Health;
	int			m_Lives;

	float m_InvincibleAccumSec{};
	float m_AbilityActivationAccumSec{};

	// Projectiles & Abilities
	Puff m_Puff;
	StarProjectile m_StarProj;
	Star m_Star;

	// Kirby Variables
	float m_WalkSpeedMultiplier;

	bool m_Bounced{false};

	bool m_IsSliding{false};
	bool m_IsRunning{false};
	bool m_IsTurning{false};
	bool m_WasInAir{false};

	bool m_IsOn30{false};
	bool m_IsOn45{false};
	bool m_IsSlopeDown{false};

	// Level
	Level* const m_pLevel;

	// Private Constants
	const float		m_JUMP_SPEED	{ 20.f - m_GRAVITY / 2.f};
	const float		m_WALK_SPEED	{ 60.f };
	const float		m_RUN_SPEED		{ 1.5f * m_WALK_SPEED };
	const float		m_SLIDE_SPEED	{ 2 * m_WALK_SPEED };
	const float		m_FLIGHT_SPEED	{ 50.f };
};

