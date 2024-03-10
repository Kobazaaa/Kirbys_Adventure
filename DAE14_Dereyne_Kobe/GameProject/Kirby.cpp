#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include <iostream>

Kirby::Kirby( const Point2f& center)
	: Entity("Kirby.png", 16, 16, center)
	, m_Health		{ 6 }
	, m_Lives		{ 4 }
{
	m_Velocity.x = 100.f;
}

Kirby::~Kirby()
{
}

void Kirby::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	// Movement
	Move(elapsedSec);
	Entity::Update(elapsedSec, world);

	// Entity (possible be moved to sprite class?)
	UpdateSprite();
	
	// World Collision
	//if (Entity::WorldCollision(world))
	//{
	//	m_CurrentState = State::None;
	//}


}

void Kirby::Draw() const
{
	if (m_Direction == Direction::Left) Entity::Draw(true);
	else Entity::Draw(false);
}

void Kirby::Move(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		  MOVEMENT			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (pStates[SDL_SCANCODE_RIGHT] and CanMoveWithCurrentState())
	{ 
		m_Direction = Direction::Right;
		m_Center.x += m_Velocity.x * elapsedSec;
		if (m_CurrentState == State::None) m_CurrentState = State::Walk;
	}
	if (pStates[SDL_SCANCODE_LEFT] and CanMoveWithCurrentState())
	{
		m_Direction = Direction::Left;
		m_Center.x -= m_Velocity.x * elapsedSec;
		if(m_CurrentState == State::None) m_CurrentState = State::Walk;
	}
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		  SLIDING			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*if (pStates[SDL_SCANCODE_DOWN] and CanSlideWithCurrentState())
	{
		m_CurrentState = State::Slide;
		if (pStates[SDL_SCANCODE_SPACE] and m_CanSlide)
		{
			m_CurrentFrame = 1;
			m_Center.x += int(m_Direction) * m_SlideSpeed * elapsedSec;
			if (m_AccumSec >= 1.f)
			{
				m_AccumSec = 0;
				m_CurrentFrame = 0;
				m_CanSlide = false;
			}
		}
		else if (!pStates[SDL_SCANCODE_SPACE]) m_CanSlide = true;
	}*/

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			JUMPING			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (pStates[SDL_SCANCODE_SPACE] and CanJumpWithCurrentState())
	{
		m_CurrentState = State::Jump;
		m_Velocity.y = m_JumpSpeed;
	}
	else if (!pStates[SDL_SCANCODE_SPACE] and m_CurrentState == State::Jump)
	{
		if (m_Velocity.y >= 0) m_Velocity.y = 0;
	}
}

#pragma region Animation
void Kirby::UpdateSprite()
{
	// Updates the active row in the sprite sheet according to Kirby's state
	m_CurrentFrameRow = int(m_CurrentState);

	// Play the correct animation according to Kirby's state
	switch (m_CurrentState)
	{
	case Kirby::State::None:
		AnimateIdle();
		break;
	case Kirby::State::Walk:
		AnimateWalk();
		break;
	case Kirby::State::Slide:
		break;
	case Kirby::State::Jump:
		AnimateJump();
		break;
	case Kirby::State::Falling:
		AnimateFall();
		break;
	case Kirby::State::Flight:
		break;
	case Kirby::State::Inhaling:
		break;
	default:
		break;
	}
}
void Kirby::AnimateIdle()
{
	if (m_AccumSec >= m_IDLE_FRAME_DELAY)
	{
		++m_CurrentFrame %= m_NR_IDLE_FRAMES;
		m_AccumSec = 0;
	}
}
void Kirby::AnimateWalk()
{
	if (m_AccumSec >= m_WALKING_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame %= m_NR_WALK_FRAMES;
	}
}
void Kirby::AnimateJump()
{
	if (m_Velocity.y < 0.f)
	{
		if (m_CurrentFrame < m_NR_FLIP_FRAMES and m_AccumSec >= m_FLIP_FRAME_DELAY)
		{
			m_AccumSec = 0.f;
			m_CurrentFrame = (m_CurrentFrame % m_NR_FLIP_FRAMES) + 1;
		}
		if (m_CurrentFrame >= m_NR_FLIP_FRAMES and m_AccumSec >= m_FALLING_DELAY)
		{
			m_AccumSec = 0.f;
			m_CurrentState = State::Falling;
		}
	}
}
void Kirby::AnimateFall()
{
	m_CurrentFrame = 0;
}
#pragma endregion

#pragma region CanXxWithCurrentState
bool Kirby::CanMoveWithCurrentState()
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Inhaling) return false;
	else return true;
}
bool Kirby::CanJumpWithCurrentState()
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Inhaling or
		m_CurrentState == State::Falling or
		m_CurrentState == State::Jump) return false;
	else return true;
}
bool Kirby::CanSlideWithCurrentState()
{
	if (m_CurrentState == State::None) return true;
	else return false;
}
#pragma endregion

