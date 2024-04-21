#pragma once
#include <vector>
#include "Entity.h"
#include "Vector2f.h"
#include "Texture.h"

class Enemy;
class Level;

class Kirby final : public Entity
{
public:
	enum class State
	{
		None, Walk, Slide, Jump, Falling, Flight, Inhaling, Exhaling, Swallow
	};

	// Constructor & Destructor
	explicit Kirby(const Point2f& center, Level* const level);
	virtual ~Kirby() override = default;
	friend std::ostream& operator<<(std::ostream& out, const Kirby& kirby);
	std::string EnumToString(Kirby::State state) const;

	// Behavioral
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	void Draw() const;
	void HitEnemy();
	void InhaledEnemy(Enemy* enemy);
	bool DoDoorChecks();

	void Reset();

	// Accessors
	Kirby::State GetCurrentState()  const;
	Rectf		 GetInhaleRect()	const;
	int			 GetHealth()		const;
	int			 GetLives()			const;
	int			 GetScore()			const;

	// Mutators

private:
	// Private Update Functions
	void UpdateSprite();
	void MovementUpdate(float elapsedSec);
	void ApplyPlaySpace();

	// Animation Functions
	void AnimateIdle();
	void AnimateWalk();
	void AnimateJump();
	void AnimateSlide();
	void AnimateFall();
	void AnimateFlight();
	void AnimateInhaling();
	void AnimateExhaling();
	void AnimateSwallow();

	// Functions that check whether or not Kirby can perform a certain action in his current state
	bool CanMoveWithCurrentState()		const;
	bool CanJumpWithCurrentState()		const;
	bool CanSlideWithCurrentState()		const;
	bool CanFlyWithCurrentState()		const;
	bool CanInhaleWithCurrentState()	const;

	// Timers
	const float m_INVINCIBILITY_TIME	{3.f};
	const float m_SLIDING_TIME			{0.5f};

	// Frame Delays for Animations (in seconds)
	const float m_IDLE_START_DELAY		{ 1.f };
	const float m_IDLE_FRAME_DELAY		{ 0.5f };

	const float m_WALKING_FRAME_DELAY	{ 0.1f };

	const float m_FLIP_FRAME_DELAY		{ 0.07f };
	const float m_FALLING_DELAY			{0.35f};
	const float m_JUMP_FRAME_DELAY_FULL	{0.1f};

	const float m_FLIGHT_FRAME_DELAY	{ 0.1f };

	const float m_INHALE_FRAME_DELAY	{ 0.1f };
	const float m_EXHALE_FRAME_DELAY	{ 0.15f };

	const float m_SWALLOW_FRAME_DELAY	{ 0.15f };

	// Nr. of frames for Animations
	const int m_NR_IDLE_FRAMES{ 2 };

	const int m_NR_WALK_FRAMES{ 4 };

	const int m_NR_FLIP_FRAMES{ 4 };
	const int m_NR_JUMP_FRAMES_FULL{ 2 };

	const int m_NR_FLIGHT_FRAMES{ 10 };

	const int m_NR_SWALLOW_FRAMES{ 3 };

	// Private Data Members
	State		m_CurrentState{State::None};
	State		m_OldState{State::None};

	const float		m_JUMP_SPEED	{ 20.f - m_GRAVITY / 2.f};
	const float		m_WALK_SPEED	{ 60.f};
	const float		m_RUN_SPEED		{ 2 * m_WALK_SPEED };
	const float		m_SLIDE_SPEED	{ m_RUN_SPEED };
	const float		m_FLIGHT_SPEED	{ 50.f };
	
	int			m_Health;
	int			m_Lives;
	int			m_Score;

	float m_InvincibleAccumSec{};

	bool m_InhaledEnemy;
	Enemy* m_pInhaledEnemy;

	bool m_IsSliding{false};

	Level* const m_pLevel;
};

