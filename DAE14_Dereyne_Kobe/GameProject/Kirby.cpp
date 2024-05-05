#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include "Enemy.h"
#include "Level.h"
#include <iostream>
#include <typeinfo>

using namespace utils;

Kirby::Kirby( const Point2f& center, Level* const level)
	: Entity("Kirby", 24, 24, center)
	, m_Health			{ 6 }
	, m_Lives			{ 4 }
	, m_InhaledEnemy	{ false }
	, m_pInhaledEnemy	{ nullptr }
	, m_Score			{ 0 }
	, m_pLevel			{ level }
	, m_Puff			{ Puff()}
	, m_StarProj		{ StarProjectile() }
	, m_Card			{ Card::Ability }
	, m_InvincibleAccumSec{0}
	, m_AbilityActivationAccumSec{0}
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
	case Kirby::State::Swallow:
		enumAsString = "Swallow";
		break;
	default:
		enumAsString = "[NO STATE FOUND]";
		break;
	}

	return enumAsString;
}

void Kirby::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	// Ability Usage
	if (m_AbilityType != AbilityType::None)
	{
		m_pAbility->Update(elapsedSec, world, this);
		if (m_pAbility->IsActive())
		{
			m_AbilityActivationAccumSec += elapsedSec;
			m_CurrentState = State::Ability;
		}
	}

	m_Puff.Update(elapsedSec, world);
	m_StarProj.Update(elapsedSec, world);

	// Invincibility
	if (m_IsInvincible) m_InvincibleAccumSec += elapsedSec;
	if (m_IsInvincible and m_InvincibleAccumSec >= m_INVINCIBILITY_TIME)
	{
		m_InvincibleAccumSec = 0;
		m_IsInvincible = false;
	}

	if (m_Health <= 0)
	{
		--m_Lives;
		m_Health = 6;
		if (m_Lives <= 0)
		{
			Reset();
		}
	}

	// Movement
	MovementUpdate(elapsedSec);
	Entity::Update(elapsedSec, world);
	// Animate
	UpdateSprite();


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		 IN/EXHALING		~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (KeyPress(SDL_SCANCODE_LSHIFT))
	{
		if (m_InhaledEnemy)
		{
			m_CurrentState = State::Exhaling;
			if (!m_StarProj.IsActivated())
			{
				m_StarProj.Activate(m_Position, m_Direction);
			}
		}
		else if (m_CurrentState == State::Flight)
		{
			m_CurrentState = State::Exhaling;
			if (!m_Puff.IsActivated())
			{
				m_Puff.Activate(m_Position, m_Direction);
			}
		}
		else if (m_AbilityType != AbilityType::None)
		{
			m_pAbility->Activate(m_Position, m_Direction);
		}
		else if (CanInhaleWithCurrentState()) m_CurrentState = State::Inhaling;
	}

	if (KeyDown(SDL_SCANCODE_LSHIFT))
	{
		if (m_AbilityType != AbilityType::None and m_pAbility->IsActive() and m_pAbility->CanBeCancelled())
		{
			m_pAbility->Activate();
		}
	}
	else
	{
		if (m_AbilityType != AbilityType::None and m_pAbility->CanBeCancelled() and m_AbilityActivationAccumSec >= 1.f)
		{
			m_AbilityActivationAccumSec = 0;
			m_pAbility->Deactivate();
		}
	}

	if (KeyPress(SDL_SCANCODE_RSHIFT))
	{
		delete m_pAbility;
		m_pAbility = nullptr;
		m_AbilityType = AbilityType::None;
	}

	if (!KeyDown(SDL_SCANCODE_LSHIFT) and m_CurrentState == State::Inhaling and m_AccumSec >= 0.40f)
	{
		m_CurrentState = State::None;
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			SWALLOW			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (m_InhaledEnemy and m_CurrentState == State::None and KeyPress(SDL_SCANCODE_DOWN))
	{
		delete m_pAbility;

		if (m_pInhaledEnemy->GetAbilityType() != AbilityType::None)
		{
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Beam)		m_pAbility = new Beam(true);
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Fire)		m_pAbility = new Fire(true);
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Spark)	m_pAbility = new Spark(true);
			m_AbilityType = m_pInhaledEnemy->GetAbilityType();
		}

		m_CurrentState = State::Swallow;
		m_InhaledEnemy = false;
		m_pInhaledEnemy = nullptr;
	}


	// World Collision
	ApplyPlaySpace();
	if (Collision::WallCollision(this, world))
	{
		m_IsSliding = false;
		if (m_CurrentState == State::Walk and !m_InhaledEnemy)
		{
			m_CurrentFrame = 9;
			m_CurrentState = State::None;
		}
	}
	if (Collision::FloorCollision(this, world))
	{
		if (m_CurrentState != State::Exhaling and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Falling and
			m_CurrentState != State::Swallow and
			(m_Velocity.x >= 0.f )) m_CurrentState = State::None;

		if (m_CurrentState == State::Falling)
		{
			static bool bounced{false};
			if (bounced)
			{
				m_CurrentState = State::None;
				bounced = false;
			}
			else
			{
				m_Velocity.y = m_JUMP_SPEED / 2;
				bounced = true;
			}
		}
	}
	else
	{
		if (m_Velocity.y < m_GRAVITY / 1.75f and (m_CurrentState == State::None or m_CurrentState == State::Jump or m_CurrentState == State::Falling or m_CurrentState == State::Walk) and !m_InhaledEnemy)
		{
			m_CurrentState = State::Falling;
		}

		if (m_Velocity.y < 0.f and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Exhaling and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Jump and
			m_CurrentState != State::Falling)
		{
			m_CurrentState = State::None;
			m_CurrentFrameRow = static_cast<int>(State::Jump);
			if (!m_InhaledEnemy) m_CurrentFrame = 4;
			else m_CurrentFrame = 2;
		}

	}
}

void Kirby::Draw() const
{
	m_Puff.Draw();
	m_StarProj.Draw();

	if (m_Direction == Direction::Left) Entity::Draw(true);
	else Entity::Draw(false);
}

void Kirby::MovementUpdate(float elapsedSec)
{
	if (m_pAbility != nullptr and m_pAbility->IsActive())
	{
		m_IsSliding = false;

		const float FRICTION{ -250.f };
		m_Velocity.x += elapsedSec * FRICTION;
		if (m_Velocity.x <= 0.f)
		{
			m_Velocity.x = 0;

		}
		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
	}

	if ((m_pAbility != nullptr and !m_pAbility->IsActive()) or m_pAbility == nullptr)
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
			if (m_IsSliding)
			{
				if (m_AccumSec >= m_SLIDING_TIME)
				{
					m_Velocity.x = 0;
					m_IsSliding = false;
				}
				else m_Velocity.x = m_SLIDE_SPEED;
			}
		}



		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~	DIRECTIONAL MOVEMENT	~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ((KeyDown(SDL_SCANCODE_RIGHT) or KeyDown(SDL_SCANCODE_LEFT)) and CanMoveWithCurrentState())
		{
			if (m_CurrentState != State::Falling)
			{
				m_Velocity.x += m_WALK_SPEED * (elapsedSec * 5);
				if (m_Velocity.x >= m_WALK_SPEED) m_Velocity.x = m_WALK_SPEED;
				m_Direction = KeyDown(SDL_SCANCODE_RIGHT) ? Direction::Right : Direction::Left;
			}
		}
		else
		{
			const float FRICTION{ -250.f };
			m_Velocity.x += elapsedSec * FRICTION;
			if (m_Velocity.x <= 0.f)
			{
				m_Velocity.x = 0;
			}
		}

		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		if (m_Velocity.x >= 5.f and m_CurrentState == State::None) m_CurrentState = State::Walk;



		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~			JUMPING			~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (KeyPress(SDL_SCANCODE_SPACE) and CanJumpWithCurrentState())
		{
			m_CurrentState = State::Jump;
			m_Velocity.y = m_JUMP_SPEED;
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
			m_Velocity.y = m_FLIGHT_SPEED;
		}
	}
}

void Kirby::ApplyPlaySpace()
{
	Rectf subLevel
	{
		0,
		m_pLevel->GetCurrentSubLevel() * m_pLevel->GetSubLevelHeight(),
		m_pLevel->GetWidth(),
		m_pLevel->GetSubLevelHeight()
	};

	if (m_Position.x - GetHitBox().width  / 2 < subLevel.left)							m_Position.x = subLevel.left					 + GetHitBox().width  / 2;
	if (m_Position.x + GetHitBox().width  / 2 > subLevel.left + subLevel.width)			m_Position.x = subLevel.left + subLevel.width	 - GetHitBox().width  / 2;
	if (m_Position.y + GetHitBox().height / 2 > subLevel.bottom + subLevel.height) 
	{
		m_Position.y = subLevel.bottom + subLevel.height - GetHitBox().height / 2;
		m_Velocity.y = 0;
	}
	if (m_Position.y + GetHitBox().height / 2 < subLevel.bottom)						m_Health = 0;
}

bool Kirby::DoDoorChecks()
{
	std::vector<Door> vDoors = m_pLevel->GetDoors();
	for (const Door& door : vDoors)
	{
		if (utils::IsPointInRect(m_Position, door.doorRect))
		{
			if (KeyPress(SDL_SCANCODE_UP) and !door.isFinalDoor)
			{

				if (m_CurrentState != State::Flight and !m_InhaledEnemy and m_CurrentState != State::Exhaling)
				{
					if (door.outcomePos.y > m_Position.y) m_pLevel->IncreaseSubLevel();
					else m_pLevel->DecreaseSubLevel();
					m_Position = door.outcomePos;
					return true;
				}
				m_CurrentState = State::Exhaling;
			}
		}
	}
	return false;
}

void Kirby::Reset()
{
	m_pLevel->SetSubLevel(0);
	m_Position = Point2f(50, 50);
	m_Lives = 4;
	m_Health = 6;
	m_Score = 0;
	m_IsInvincible = false;
	m_InhaledEnemy = false;
	m_pInhaledEnemy = nullptr;
	delete m_pAbility;
	m_pAbility = nullptr;
	m_CurrentState = State::None;
	m_AccumSec = 0;
	m_InvincibleAccumSec = 0;
}

void Kirby::HitEnemy()
{
	if (!m_IsInvincible)
	{
		--m_Health;
		m_IsInvincible = true;
	}
}

void Kirby::InhaledEnemy(Enemy* enemy)
{
	m_pInhaledEnemy = enemy;
	m_InhaledEnemy = true;
	m_CurrentState = State::None;
	m_Score += ((rand() % 5) + 1) * 100;
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
Kirby::Card Kirby::GetCard() const
{
	return m_Card;
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

	// Updates the active row in the sprite sheet according to Kirby's state
	m_CurrentFrameRow = static_cast<int>(m_CurrentState);

	// Set CurrentFrame and AccumSec to 0 when a change in State is detected
	if (m_CurrentState != m_OldState)
	{
		m_CurrentFrame = 0;
		m_OldState = m_CurrentState;
		m_AccumSec = 0;
	}
	

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
	case Kirby::State::Swallow:
		AnimateSwallow();
		break;
	case Kirby::State::Ability:
		AnimateAbility();
		break;
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
	//if (!m_InhaledEnemy)
	//{
		if (m_AccumSec >= m_WALKING_FRAME_DELAY)
		{
			m_AccumSec = 0;
			++m_CurrentFrame %= m_NR_WALK_FRAMES;
		}
	//}
	//else
	//{
	//	if (m_AccumSec >= m_WALKING_FRAME_DELAY)
	//	{
	//		m_AccumSec = 0;
	//		++m_CurrentFrame %= m_NR_WALK_FRAMES;
	//	}
	//}
}
void Kirby::AnimateJump()
{
	if (!m_InhaledEnemy)
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
	else
	{
		if (m_AccumSec >= m_JUMP_FRAME_DELAY_FULL)
		{
			m_AccumSec = 0;
			++m_CurrentFrame;
			if (m_CurrentFrame > m_NR_JUMP_FRAMES_FULL) m_CurrentFrame = m_NR_JUMP_FRAMES_FULL;
		}
	}
}
void Kirby::AnimateSlide()
{
	if (m_Velocity.x > m_WALK_SPEED) m_CurrentFrame = 1;
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
		if (m_AccumSec >= 3 * m_FLIGHT_FRAME_DELAY)
		{
			m_AccumSec = 0;
			m_CurrentFrame = (m_CurrentFrame + 1) % 2 + 4;
		}
	}
	else if (m_AccumSec >= m_FLIGHT_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame %= m_NR_FLIGHT_FRAMES;
		if (m_Velocity.y > 0.f and m_CurrentFrame > 5) m_CurrentFrame = 4;

	}
}
void Kirby::AnimateInhaling()
{
	if (m_AccumSec > m_INHALE_FRAME_DELAY)
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
	if (m_AccumSec >= m_EXHALE_FRAME_DELAY)
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
void Kirby::AnimateSwallow()
{
	if (m_AccumSec >= m_SWALLOW_FRAME_DELAY)
	{
		m_AccumSec = 0;
		++m_CurrentFrame;

		if (m_CurrentFrame >= m_NR_SWALLOW_FRAMES) m_CurrentState = State::None;
	}
}
void Kirby::AnimateAbility()
{
	if (m_AccumSec >= 0.1)
	{
		if (m_AbilityType == AbilityType::Beam)
		{
			m_AccumSec = 0;
			m_CurrentFrame = (m_CurrentFrame + 1) % 2;
		}
		if (m_AbilityType == AbilityType::Spark)
		{
			m_AccumSec = 0;
			m_CurrentFrame = ((m_CurrentFrame + 1) % 3) + 2;
		}
		if (m_AbilityType == AbilityType::Fire)
		{
			m_AccumSec = 0;
			m_CurrentFrame = ((m_CurrentFrame + 1) % 2) + 5;
		}
	}
}
#pragma endregion

#pragma region CanXxWithCurrentState
bool Kirby::CanMoveWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Swallow or
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
		m_CurrentState == State::Swallow	or
		m_Velocity.y != 0.f) return false;
	else return true;
}
bool Kirby::CanSlideWithCurrentState() const
{
	if (m_InhaledEnemy) return false;
	if (m_CurrentState == State::None or
		m_CurrentState == State::Slide) return true;
	else return false;
}
bool Kirby::CanFlyWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Exhaling or
		m_CurrentState == State::Swallow or
		m_InhaledEnemy) return false;
	else return true;
}
bool Kirby::CanInhaleWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Flight or
		m_CurrentState == State::Exhaling or
		m_CurrentState == State::Swallow or
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
