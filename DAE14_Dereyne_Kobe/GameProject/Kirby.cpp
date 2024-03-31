#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include "Level.h"
#include <iostream>

Kirby::Kirby( const Point2f& center, Level* const level)
	: Entity("Kirby.png", 24, 24, center)
	, m_Health		{ 6 }
	, m_Lives		{ 4 }
	, m_InhaledEnemy{ false }
	, m_Score		{ 1'234'567 }
	, m_pLevel		{ level }
{
}

void Kirby::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	UpdateHitBox();

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (m_IsInvincible) m_InvincibleAccumSec += elapsedSec;
	if (m_IsInvincible and m_InvincibleAccumSec >= 3.f)
	{
		m_InvincibleAccumSec = 0;
		m_IsInvincible = false;
		std::cout << "Set Invincible to False\n";
	}

	if (pStates[SDL_SCANCODE_U]) ++m_Score;
	if (pStates[SDL_SCANCODE_D]) --m_Score;

	// Movement
	MovementUpdate(elapsedSec, pStates);
	Entity::Update(elapsedSec, world);

	//if (m_Velocity.y < m_GRAVITY / 2 and m_CurrentState != State::Flight)
	//{
	//	m_CurrentState = State::Falling;
	//}

	// Entity (possible be moved to sprite class?)
	UpdateSprite();
	
	// World Collision
	if (m_WorldFloorCollision and m_CurrentState != State::Flight and m_CurrentState != State::Inhaling)
	{
		if (m_CurrentState != State::Exhaling) m_CurrentState = State::None;
	}
	if (Collision::WallCollision(this, world));

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		 IN/EXHALING		~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (pStates[SDL_SCANCODE_LSHIFT] and CanInhaleWithCurrentState())
	{
		m_CurrentState = State::Inhaling;
	}
	//else if (pStates[SDL_SCANCODE_LSHIFT] and (m_InhaledEnemy or m_CurrentState == State::Flight) and m_OldState != State::Inhaling)
	//{
	//	m_CurrentState = State::Exhaling;
	//}
	if (!pStates[SDL_SCANCODE_LSHIFT] and m_CurrentState == State::Inhaling and m_AccumSec >= 0.40f)
	{
		m_CurrentState = State::None;
	}

	if (pStates[SDL_SCANCODE_R])
	{
		m_CurrentState = State::None;
		m_InhaledEnemy = false;
	}

	DoDoorChecks();
}

void Kirby::Draw() const
{
	if (m_Direction == Direction::Left) Entity::Draw(true);
	else Entity::Draw(false);
}

void Kirby::MovementUpdate(float elapsedSec, const Uint8* pStates)
{
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	DIRECTIONAL MOVEMENT	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ((pStates[SDL_SCANCODE_RIGHT] or pStates[SDL_SCANCODE_LEFT]) and CanMoveWithCurrentState())
	{
		m_Velocity.x = 60.f;
		m_Direction = pStates[SDL_SCANCODE_RIGHT] ? Direction::Right : Direction::Left;
	}
	else
	{
		const float FRICTION{ -15.f };
		m_Velocity.x += m_Velocity.x * elapsedSec * FRICTION;
		if (m_Velocity.x <= 0.f)
		{
			m_Velocity.x = 0.f;
		}
	}
	m_SpriteCenter.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
	if (m_Velocity.x >= 5.f and m_CurrentState == State::None) m_CurrentState = State::Walk;

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

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			FLIGHT			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ((pStates[SDL_SCANCODE_UP] and CanFlyWithCurrentState()) or (pStates[SDL_SCANCODE_SPACE] and m_CurrentState == State::Flight))
	{
		m_CurrentState = State::Flight;
		m_Velocity.y = 50.f;
	}
}

void Kirby::UpdateHitBox()
{
	m_HitBox.left = m_SpriteCenter.x - 8.f;
	m_HitBox.bottom = m_SpriteCenter.y - 12.f;
	m_HitBox.width = 16.f;
	m_HitBox.height = 16.f;
}

void Kirby::DoDoorChecks()
{
	if (utils::IsPointInRect(m_SpriteCenter, Rectf(968, 125, 16, 24)))
	{
		m_SpriteCenter = Point2f(50, 200);
	}
}

void Kirby::HitEnemy()
{
	if (!m_IsInvincible)
	{
		--m_Health;
		m_IsInvincible = true;
		std::cout << "Set Invincible to True\n";
		std::cout << m_Health << std::endl;
	}
}

void Kirby::InhaledEnemy()
{
	m_InhaledEnemy = true;
	m_CurrentState = State::None;
}

#pragma region Accessors
Kirby::State Kirby::GetCurrentState() const
{
	return m_CurrentState;
}
int Kirby::GetHealth() const
{
	return m_Health;
}
int Kirby::GetLives() const
{
	return m_Lives;
}
int Kirby::GetScore() const
{
	return m_Score;
}
Rectf Kirby::GetInhaleRect() const
{
	Rectf inhaleRect
	{
		m_SpriteCenter.x + (((static_cast<int>(m_Direction) + 1) / 2) - 1) * 48.f,
		m_SpriteCenter.y - m_Height / 2 - 16.f,
		48.f, 48.f
	};
	return inhaleRect;
}
#pragma endregion

#pragma region Animation
void Kirby::UpdateSprite()
{
	if (m_InhaledEnemy) m_SrcRectStart = Point2f(240, 0);
	else m_SrcRectStart = Point2f(0, 0);

	// Set CurrentFrame and AccumSec to 0 when a change in State is detected
	if (m_CurrentState != m_OldState)
	{
		m_CurrentFrame = 0;
		m_OldState = m_CurrentState;
		m_AccumSec = 0;
	}
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
		AnimateFlight();
		break;
	case Kirby::State::Inhaling:
		AnimateInhaling();
		break;
	case Kirby::State::Exhaling:
		AnimateExhaling();
	default:
		break;
	}

}
void Kirby::AnimateIdle()
{
	m_CurrentFrame = 0;
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
	if (m_Velocity.y >= 0.f) m_CurrentFrame = 0;
	else
	{
		if (m_CurrentFrame < m_NR_FLIP_FRAMES and m_AccumSec >= m_FLIP_FRAME_DELAY)
		{
			m_AccumSec = 0.f;
			m_CurrentFrame = (m_CurrentFrame % m_NR_FLIP_FRAMES) + 1;
		}
		//if (m_CurrentFrame >= m_NR_FLIP_FRAMES and m_AccumSec >= m_FALLING_DELAY)
		//{
		//	m_AccumSec = 0.f;
		//	m_CurrentState = State::Falling;
		//}
	}
}
void Kirby::AnimateFall()
{
	m_CurrentFrame = 0;
}
void Kirby::AnimateFlight()
{
	if (m_Velocity.y <= 0.f)
	{
		if (m_AccumSec >= 0.3f)
		{
			m_AccumSec = 0;
			m_CurrentFrame = (m_CurrentFrame + 1) % 2 + 4;
		}
	}
	else if (m_AccumSec >= 0.1f)
	{
		m_AccumSec = 0;
		++m_CurrentFrame %= m_NR_FLIGHT_FRAMES;
		if (m_Velocity.y > 0.f and m_CurrentFrame > 5) m_CurrentFrame = 4;

	}
}
void Kirby::AnimateInhaling()
{
	if (m_AccumSec > 0.1f)
	{
		if (m_CurrentFrame == 1) m_CurrentFrame = 1;
		else
		{
			m_AccumSec = 0;
			++m_CurrentFrame;
		}
	}
}
void Kirby::AnimateExhaling()
{
	if (m_AccumSec >= 0.25f)
	{
		m_AccumSec -= 0.1f;
		++m_CurrentFrame;
		if (m_CurrentFrame >= 4)
		{
			m_InhaledEnemy = false;
			m_CurrentState = State::None;
		}
	}
}
#pragma endregion

#pragma region CanXxWithCurrentState
bool Kirby::CanMoveWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Inhaling) return false;
	else return true;
}
bool Kirby::CanJumpWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Inhaling	or
		m_CurrentState == State::Falling	or
		m_CurrentState == State::Jump		or
		m_CurrentState == State::Flight		or
		m_CurrentState == State::Exhaling	or
		m_Velocity.y != 0.f) return false;
	else return true;
}
bool Kirby::CanSlideWithCurrentState() const
{
	if (m_CurrentState == State::None) return true;
	else return false;
}
bool Kirby::CanFlyWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Exhaling or
		m_InhaledEnemy) return false;
	else return true;
}
bool Kirby::CanInhaleWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Flight or
		(m_CurrentState == State::Exhaling) or
		m_InhaledEnemy) return false;
	else return true;
}
#pragma endregion

