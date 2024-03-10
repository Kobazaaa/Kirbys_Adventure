#pragma once
#include <vector>
#include "Entity.h"
#include "Vector2f.h"
#include "Texture.h"

class Kirby : public Entity
{
public:
	enum class State
	{
		None, Walk, Slide, Jump, Falling, Flight, Inhaling
	};

	// Constructors and Destructor
	Kirby(const Point2f& center );
	~Kirby();

	// Behavioral Functions
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);
	void Draw() const;
private:
	// Private Update Functions
	void UpdateSprite();
	void Move(float elapsedSec);

	// Animation Functions
	void AnimateIdle();
	void AnimateWalk();
	void AnimateJump();
	void AnimateFall();

	// Functions that check whether or not Kirby can perform a certain action in his current state
	bool CanMoveWithCurrentState();
	bool CanJumpWithCurrentState();
	bool CanSlideWithCurrentState();

	// Frame Delays for Animations (in seconds)
	const float m_IDLE_FRAME_DELAY{ 1.f };
	const float m_WALKING_FRAME_DELAY{ 0.1f };
	const float m_FLIP_FRAME_DELAY{ 0.07f };
	const float m_FALLING_DELAY{1.f};

	// Nr. of frames for Animations
	const int m_NR_IDLE_FRAMES{ 2 };
	const int m_NR_WALK_FRAMES{ 4 };
	const int m_NR_FLIP_FRAMES{ 4 };

	// Private Data Members
	State		m_CurrentState{State::None};

	float		m_JumpSpeed{300.f};
	float		m_SlideSpeed{100.f};
	int			m_Health;
	int			m_Lives;

	bool m_CanSlide{true};
};

