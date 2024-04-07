#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include "Level.h"
#include <iostream>

using namespace utils;

Kirby::Kirby( const Point2f& center, Level* const level)
	: Entity("Kirby.png", 24, 24, center)
	, m_Health		{ 6 }
	, m_Lives		{ 4 }
	, m_InhaledEnemy{ false }
	, m_Score		{ 1'234'567 }
	, m_pLevel		{ level }
{
}

std::string Kirby::EnumToString(Kirby::State state) const
{
	std::string enumAsString{};

	switch (state)
	{
	case Kirby::State::None:
		enumAsString = "None";
		break;
	case Kirby::State::Walk:
		enumAsString = "Walk";
		break;
	case Kirby::State::Slide:
		enumAsString = "Slide";
		break;
	case Kirby::State::Jump:
		enumAsString = "Jump";
		break;
	case Kirby::State::Falling:
		enumAsString = "Falling";
		break;
	case Kirby::State::Flight:
		enumAsString = "Flight";
		break;
	case Kirby::State::Inhaling:
		enumAsString = "Inhaling";
		break;
	case Kirby::State::Exhaling:
		enumAsString = "Exhaling";
		break;
	case Kirby::State::Full:
		enumAsString = "Full";
		break;
	default:
		enumAsString = "[NO STATE FOUND]";
		break;
	}

	return enumAsString;
}

void Kirby::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	// Invincibility
	if (m_IsInvincible) m_InvincibleAccumSec += elapsedSec;
	if (m_IsInvincible and m_InvincibleAccumSec >= 3.f)
	{
		m_InvincibleAccumSec = 0;
		m_IsInvincible = false;
		std::cout << "Set Invincible to False\n";
	}

	// Score
	if (KeyDown(SDL_SCANCODE_U)) ++m_Score;
	if (KeyDown(SDL_SCANCODE_D)) --m_Score;

	// Movement
	MovementUpdate(elapsedSec);
	Entity::Update(elapsedSec, world);

	if (m_Velocity.y < m_GRAVITY / 1.85f and (m_CurrentState == State::None or m_CurrentState == State::Jump))
	{
		m_CurrentState = State::Falling;
	}

	// Animate
	UpdateSprite();
	
	// World Collision
	if (Collision::FloorCollision(this, world))
	{
		if (m_CurrentState != State::Exhaling and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Walk or
			m_Velocity.x <= 0) m_CurrentState = State::None;
	}
	if (Collision::WallCollision(this, world))
	{
		std::cout << "Wall\n";
		if (m_CurrentState == State::Walk)
		{
			m_CurrentFrame = 9;
			m_CurrentState = State::None;
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		 IN/EXHALING		~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (KeyDown(SDL_SCANCODE_LSHIFT) and CanInhaleWithCurrentState())
	{
		m_CurrentState = State::Inhaling;
	}	
	if (m_InhaledEnemy and KeyPress(SDL_SCANCODE_LSHIFT))
	{
		m_CurrentState = State::Exhaling;
	}

	if (!KeyDown(SDL_SCANCODE_LSHIFT) and m_CurrentState == State::Inhaling and m_AccumSec >= 0.40f)
	{
		m_CurrentState = State::None;
	}


	// Debugging Purposes
	if (KeyDown(SDL_SCANCODE_R))
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

void Kirby::MovementUpdate(float elapsedSec)
{
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			SLIDE			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ((KeyDown(SDL_SCANCODE_DOWN) and CanSlideWithCurrentState()) or m_IsSliding)
	{
		m_CurrentState = State::Slide;
		if (KeyPress(SDL_SCANCODE_SPACE) and !m_IsSliding)
		{
			m_AccumSec = 0;
			m_IsSliding = true;
		}
		if (m_IsSliding and m_AccumSec >= 0.5f) m_IsSliding = false;
		else if (m_IsSliding) m_Velocity.x = 120.f;
	}
	else
	{
		m_IsSliding = false;
	}



	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	DIRECTIONAL MOVEMENT	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ((KeyDown(SDL_SCANCODE_RIGHT) or KeyDown(SDL_SCANCODE_LEFT)) and CanMoveWithCurrentState())
	{
		m_Velocity.x = 60.f;
		m_Direction = KeyDown(SDL_SCANCODE_RIGHT) ? Direction::Right : Direction::Left;
	}
	else
	{
		const float FRICTION{ -250.f };
		m_Velocity.x += elapsedSec * FRICTION;
		if (m_Velocity.x <= 0.f)
		{
			m_Velocity.x = 0.f;
		}
	}
	
	m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
	if (m_Velocity.x >= 5.f and m_CurrentState == State::None) m_CurrentState = State::Walk;



	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			JUMPING			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (KeyDown(SDL_SCANCODE_SPACE) and CanJumpWithCurrentState())
	{
		m_CurrentState = State::Jump;
		m_Velocity.y = m_JumpSpeed;
	}
	else if (KeyRelease(SDL_SCANCODE_SPACE))
	{
		if (m_Velocity.y >= 0) m_Velocity.y = 0;
	}



	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			FLIGHT			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ((KeyDown(SDL_SCANCODE_UP) and CanFlyWithCurrentState()) or (KeyDown(SDL_SCANCODE_SPACE) and m_CurrentState == State::Flight))
	{
		m_CurrentState = State::Flight;
		m_Velocity.y = 50.f;
	}
	if (m_CurrentState == State::Flight and KeyPress(SDL_SCANCODE_LSHIFT))
	{
		m_CurrentState = State::Exhaling;
	}



}

void Kirby::DoDoorChecks()
{
	if (utils::IsPointInRect(m_Position, Rectf(968, 125, 16, 24)))
	{
		m_Position = Point2f(50, 200);
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

void Kirby::InhaledEnemy(Enemy* enemy)
{
	m_InhaledEnemy = true;
	m_pInhaledEnemy = enemy;
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
		m_Position.x + (((static_cast<int>(m_Direction) + 1) / 2) - 1) * 48.f,
		m_Position.y - m_Height / 2 - 16.f,
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
		AnimateSlide();
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
void Kirby::AnimateSlide()
{
	if (m_Velocity.x > 60.f) m_CurrentFrame = 1;
	else m_CurrentFrame = 0;
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
	if (m_AccumSec >= 0.15f)
	{
		m_AccumSec -= 0.1f;
		++m_CurrentFrame;
		if (m_CurrentFrame >= 4)
		{
			m_InhaledEnemy = false;
			m_pInhaledEnemy = nullptr;
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
	if (m_CurrentState == State::None or
		m_CurrentState == State::Slide) return true;
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

std::ostream& operator<<(std::ostream& out, const Kirby& kirby)
{
	std::string currentState{ kirby.EnumToString(kirby.m_CurrentState) };
	std::string oldState{ kirby.EnumToString(kirby.m_OldState) };
	

	out << "Position: (" << kirby.m_Position.x << ", " << kirby.m_Position.y << ")\n"
		<< "Current State:" << currentState << "\tOld State: " << oldState << std::endl
		<< "Lives: " << kirby.m_Lives << "\tHealth: " << kirby.m_Health << "\t Score: " << kirby.m_Score << std::endl
		<< "Invinclibe: " << std::boolalpha << kirby.m_IsInvincible << std::endl << std::endl << std::endl;

	return out;
}
