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
		None, Walk, Slide, Jump, Falling, Flight, Inhaling, Exhaling, Full
	};

	// Constructor & Destructor
	Kirby(const Point2f& center, Level* const level);
	virtual ~Kirby() override;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	void Draw() const;
	void EnemyCollision();
	void InhalingEnemy();

	// Accessors
	Rectf GetInhaleRect() const;
	Kirby::State GetCurrentState() const;
	int GetHealth() const;
	int GetLives() const;
	int GetScore() const;

private:
	// Private Update Functions
	void UpdateSprite();
	void MovementUpdate(float elapsedSec, const Uint8* pStates);
	
	void DoDoorChecks();

	//bool FloorCollision(const std::vector<std::vector<Point2f>>& world);

	// Animation Functions
	void AnimateIdle();
	void AnimateWalk();
	void AnimateJump();
	void AnimateFall();
	void AnimateFlight();
	void AnimateInhaling();
	void AnimateExhaling();

	// Functions that check whether or not Kirby can perform a certain action in his current state
	bool CanMoveWithCurrentState() const;
	bool CanJumpWithCurrentState() const;
	bool CanSlideWithCurrentState() const;
	bool CanFlyWithCurrentState() const;
	bool CanInhaleWithCurrentState() const;

	// Frame Delays for Animations (in seconds)
	const float m_IDLE_START_DELAY{ 1.f };
	const float m_IDLE_FRAME_DELAY{ 0.5f };

	const float m_WALKING_FRAME_DELAY{ 0.1f };

	const float m_FLIP_FRAME_DELAY{ 0.07f };
	const float m_FALLING_DELAY{0.35f};

	const float m_FLIGHT_FRAME_DELAY{ 0.1f };

	// Nr. of frames for Animations
	const int m_NR_IDLE_FRAMES{ 2 };
	const int m_NR_WALK_FRAMES{ 4 };
	const int m_NR_FLIP_FRAMES{ 4 };
	const int m_NR_FLIGHT_FRAMES{ 10 };

	// Private Data Members
	State		m_CurrentState{State::None};
	State		m_OldState{State::None};

	float		m_JumpSpeed{ 20.f - m_GRAVITY / 2.f};
	float		m_SlideSpeed{100.f};
	
	int			m_Health;
	int			m_Lives;
	int			m_Score;

	float m_InvincibleAccumSec{};

	bool m_InhaledEnemy;

	bool m_CanSlide{true};

	Level* const m_pLevel;
};

