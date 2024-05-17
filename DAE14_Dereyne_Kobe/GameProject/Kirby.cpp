#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include "Enemy.h"
#include "Level.h"
#include <iostream>
#include <typeinfo>

using namespace utils;

Kirby::Kirby( const Point2f& center, Level* const level)
	: Entity("Kirby2", 16, 16, center)
	, m_Health						{ 6 }
	, m_Lives						{ 4 }
	, m_InhaledEnemy				{ false }
	, m_pInhaledEnemy				{ nullptr }
	, m_pLevel						{ level }
	, m_Puff						{ }
	, m_StarProj					{ }
	, m_Card						{ Card::Ability }
	, m_InvincibleAccumSec			{ 0 }
	, m_AbilityActivationAccumSec	{ 0 }
	, m_WalkSpeedMultiplier			{ 1.f }
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
	AbilityUpdate(elapsedSec, world);

	if (m_IsInvincible) Invincibility(elapsedSec);
	if (m_Health <= 0) Death();

	SlopeHandling();

	MovementUpdate(elapsedSec);
	Entity::Update(elapsedSec, world);
	Animate();

	MechanicUpdate(elapsedSec);

	Collisions(world);

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
	if (m_AbilityType != AbilityType::None and m_pAbility->IsActive())
	{
		m_IsSliding = false;
		ApplyFriction(elapsedSec);
		m_Position.x += m_Velocity.x * elapsedSec;
	}

	if (m_AbilityType == AbilityType::None or !m_pAbility->IsActive())
	{
#pragma region Slide
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
					m_Velocity.x = static_cast<int>(m_Direction) * 3.f;
					m_IsSliding = false;
				}
				else m_Velocity.x = static_cast<int>(m_Direction) * m_SLIDE_SPEED;
			}
		}
#pragma endregion

#pragma region DirectionalMovement
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~	   RUNNING & TURNING	~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (m_CurrentState == State::None or m_CurrentState == State::Walk)
		{
			if (KeyPress(SDL_SCANCODE_RIGHT) and abs(m_Velocity.x) >= 5.f)
			{
				if (m_Direction == Direction::Left and !m_InhaledEnemy)
				{
					m_IsTurning = true;
					SoundManager::PlayEffect("Turn");
				}
				else m_IsTurning = false;
				if (m_Direction == Direction::Right and !m_IsTurning and !m_IsRunning) m_IsRunning = true;
				else m_IsRunning = false;
			}
			if (KeyPress(SDL_SCANCODE_LEFT) and abs(m_Velocity.x) >= 5.f)
			{
				if (m_Direction == Direction::Right and !m_InhaledEnemy)
				{
					m_IsTurning = true;
					SoundManager::PlayEffect("Turn");
				}
				else m_IsTurning = false;
				if (m_Direction == Direction::Left and !m_IsTurning and !m_IsRunning) m_IsRunning = true;
				else m_IsRunning = false;
			}
		}
		else if (m_CurrentState != State::Jump) m_IsRunning = false;

		if (m_IsTurning)
		{
			m_CurrentAnimation = "Turn";
			if (m_pAnimationManager->IsDone("Turn")) m_IsTurning = false;
		}


		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~	DIRECTIONAL MOVEMENT	~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ((KeyDown(SDL_SCANCODE_RIGHT) or KeyDown(SDL_SCANCODE_LEFT)) and CanMoveWithCurrentState())
		{
			if (!m_IsRunning)
			{
				m_Velocity.x += static_cast<int>(m_Direction) * m_WalkSpeedMultiplier * m_WALK_SPEED * (elapsedSec * 5);
				if (abs(m_Velocity.x) >= m_WalkSpeedMultiplier * m_WALK_SPEED)
				{ 
					m_Velocity.x = static_cast<int>(m_Direction) * m_WALK_SPEED * m_WalkSpeedMultiplier; 
				}
			}
			else
			{
				m_Velocity.x = static_cast<int>(m_Direction) * m_WalkSpeedMultiplier * m_RUN_SPEED;
			}
			m_Direction = KeyDown(SDL_SCANCODE_RIGHT) ? Direction::Right : Direction::Left;
		}
		else
		{
			m_IsRunning = false;
			ApplyFriction(elapsedSec);
		}
		
		if (abs(m_Velocity.x) >= 5.f and m_CurrentState == State::None) m_CurrentState = State::Walk;
		m_Position.x += m_Velocity.x * elapsedSec;
#pragma endregion

#pragma region Jumping
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~			JUMPING			~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (KeyPress(SDL_SCANCODE_SPACE))
		{
			m_Position.y += 1;
			if (CanJumpWithCurrentState())
			{
				m_CurrentState = State::Jump;
				m_Velocity.y = m_JUMP_SPEED;
				SoundManager::PlayEffect("Jump");
			}
		}
		else if (KeyRelease(SDL_SCANCODE_SPACE))
		{
			if (m_Velocity.y >= 0) m_Velocity.y = 0;
		}
#pragma endregion

#pragma region Flight
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~			FLIGHT			~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ((KeyPress(SDL_SCANCODE_UP) and CanFlyWithCurrentState()))
		{
			m_Position.y += 1;
		}

		if ((KeyDown(SDL_SCANCODE_UP) and CanFlyWithCurrentState()) or (KeyDown(SDL_SCANCODE_SPACE) and m_CurrentState == State::Flight))
		{
			if (m_CurrentState != State::EnterDoor)
			{
				if (m_CurrentState != State::Flight) m_CurrentAnimation = "FlightStart";
				m_CurrentState = State::Flight;
				m_Velocity.y = m_FLIGHT_SPEED;
			}
		}
		if (m_CurrentState == State::Flight) m_GravityMultiplier = 0.5f;
		else m_GravityMultiplier = 1.f;
	}
#pragma endregion
}
void Kirby::MechanicUpdate(float elapsedSec)
{
#pragma region In/Exhaling
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~		 IN/EXHALING		~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (KeyPress(SDL_SCANCODE_LSHIFT))
	{
		if (m_InhaledEnemy)
		{
			m_CurrentAnimation = "Exhaling";
			m_CurrentState = State::Exhaling;
			if (!m_StarProj.IsActivated())
			{
				m_StarProj.Activate(m_Position, m_Direction);
			}
		}
		else if (m_CurrentState == State::Flight)
		{
			m_CurrentAnimation = "Exhaling";
			m_CurrentState = State::Exhaling;
			if (!m_Puff.IsActivated())
			{
				m_Puff.Activate(m_Position, m_Direction);
			}
		}
		else if (m_AbilityType != AbilityType::None)
		{
			if (!m_pAbility->IsActive()) m_pAbility->Activate(m_Position, m_Direction);
		}
		else if (CanInhaleWithCurrentState())
		{
			SoundManager::PlayEffect("Inhaling");
			m_CurrentState = State::Inhaling;
		}
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

		if (m_CurrentState == State::Inhaling and m_AccumSec >= 0.40f)
		{
			m_CurrentState = State::None;
		}
	}
#pragma endregion

#pragma region RemoveAbility
	if (KeyPress(SDL_SCANCODE_RSHIFT))
	{
		delete m_pAbility;
		m_pAbility = nullptr;
		m_AbilityType = AbilityType::None;
	}
#pragma endregion

#pragma region Swallow
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			SWALLOW			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (m_InhaledEnemy and (m_CurrentState == State::None or m_CurrentState == State::Walk) and KeyPress(SDL_SCANCODE_DOWN))
	{
		m_InhaledEnemy = false;
		m_CurrentAnimation = "Swallow";
		m_CurrentState = State::Swallow;
		delete m_pAbility;

		if (m_pInhaledEnemy->GetAbilityType() != AbilityType::None)
		{
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Beam)		m_pAbility = new Beam(true);
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Fire)		m_pAbility = new Fire(true);
			if (m_pInhaledEnemy->GetAbilityType() == AbilityType::Spark)	m_pAbility = new Spark(true);
			m_AbilityType = m_pInhaledEnemy->GetAbilityType();

		}
		m_pInhaledEnemy = nullptr;
	}

	if (m_CurrentState != State::Swallow and m_OldState == State::Swallow and m_AbilityType != AbilityType::None) m_pAbility->Activate(m_Position, m_Direction);
#pragma endregion
}
void Kirby::Collisions(const std::vector<std::vector<Point2f>>& world)
{
	ApplyPlaySpace();
	if (Collision::WallCollision(this, world))
	{
		m_IsSliding = false;
		if (m_CurrentState == State::Walk and !m_InhaledEnemy)
		{
			m_CurrentAnimation = "Wall";
			if (!m_pAnimationManager->IsActive("Walk") or m_pAnimationManager->IsDone("Wall"))
			{
				m_CurrentAnimation = "Idle";
			}
			m_CurrentState = State::None;
		}
	}

	m_IsOn45 = false;
	m_IsOn30 = false;
	if (Collision::FloorCollision(this, world))
	{
		if (m_CurrentState != State::Exhaling and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Falling and
			m_CurrentState != State::Swallow and
			m_CurrentState != State::EnterDoor and
			(abs(m_Velocity.x) >= 0.f)) m_CurrentState = State::None;

		// Bounce Kirby when Falling
		if (m_CurrentState == State::Falling)
		{
			if (m_Bounced)
			{
				m_CurrentState = State::None;
				m_Bounced = false;
			}
			else
			{
				m_Position.y += 1;
				m_Velocity.y = m_JUMP_SPEED / 2;
				m_Bounced = true;
			}
		}

		// Determine on what type of slope, if any
		if (m_HitInfo.normal.DotProduct(Vector2f(0, 1)) >= sqrtf(2) / 2 + 0.1f and m_HitInfo.normal.DotProduct(Vector2f(0, 1)) < sqrtf(3) / 2 + 0.1f)
		{
			m_IsOn45 = false;
			m_IsOn30 = true;
		}
		else if (m_HitInfo.normal.DotProduct(Vector2f(0, 1)) < sqrtf(3) / 2 + 0.1f and m_HitInfo.normal.DotProduct(Vector2f(0, 1)) > 0)
		{
			m_IsOn45 = true;
			m_IsOn30 = false;
		}
	}
	else
	{
		if (m_Velocity.y < m_GRAVITY / 1.75f and (m_CurrentState == State::None or m_CurrentState == State::Jump or m_CurrentState == State::Falling or m_CurrentState == State::Walk) and !m_InhaledEnemy)
		{
			if (m_pAbility == nullptr or !m_pAbility->IsActive())
			{
				m_CurrentState = State::Falling;
			}
		}

		if (m_Velocity.y < 0.f and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Exhaling and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Jump and
			m_CurrentState != State::Falling and
			(m_AbilityType == AbilityType::None or !m_pAbility->IsActive()))
		{
			m_CurrentState = State::None;
			if (!m_InhaledEnemy) m_CurrentAnimation = "EndFlip";
		}

	}
}
void Kirby::SlopeHandling()
{
	if (m_IsOn30 or m_IsOn45)
	{
		m_IsSlopeDown = m_HitInfo.normal.DotProduct(Vector2f(static_cast<int>(m_Direction), 0)) > 0;
	}

	if (m_CurrentState == State::Flight) m_WalkSpeedMultiplier = 0.75f;
	else
	{
		if (m_IsOn30 and m_IsSlopeDown)			m_WalkSpeedMultiplier = 1.25f;
		else if (m_IsOn30 and !m_IsSlopeDown)	m_WalkSpeedMultiplier = 0.75f;
		else if (m_IsOn45 and m_IsSlopeDown)	m_WalkSpeedMultiplier = 1.5f;
		else if (m_IsOn45 and !m_IsSlopeDown)	m_WalkSpeedMultiplier = 0.5f;
		else m_WalkSpeedMultiplier = 1.f;
	}
}

void Kirby::ApplyFriction(float elapsedSec)
{
	float friction{-250.f};
	if (m_Velocity.x > 0.f)
	{
		m_Velocity.x += friction * elapsedSec;
		if (m_Velocity.x <= 0.f) m_Velocity.x = 0.f;
	}
	else if (m_Velocity.x < 0.f)
	{
		m_Velocity.x -= friction * elapsedSec;
		if (m_Velocity.x >= 0.f) m_Velocity.x = 0.f;
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
	if (m_Position.y + GetHitBox().height / 2 < subLevel.bottom) m_Health = 0;
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
				m_CurrentState = State::EnterDoor;
				m_Velocity.x = 0;
				if (m_OldState != State::Flight and !m_InhaledEnemy)
				{
					m_CurrentAnimation = "EnterDoor";
				}
				else
				{
					m_CurrentAnimation = "Exhaling";
					if (m_InhaledEnemy)
					{
						if (!m_StarProj.IsActivated())
						{
							m_StarProj.Activate(m_Position, m_Direction);
						}
					}
					else if (m_OldState == State::Flight)
					{
						if (!m_Puff.IsActivated())
						{
							m_Puff.Activate(m_Position, m_Direction);
						}
					}
				}
			}

			if (m_CurrentState == State::EnterDoor)
			{
				if(m_pAnimationManager->IsDone("Exhaling"))
				{
						m_CurrentAnimation = "EnterDoor";
				}
				if (m_pAnimationManager->IsDone("EnterDoor"))
				{
					m_CurrentState = State::None;
					m_Direction = Direction::Right;
					if (door.outcomePos.y > m_Position.y) m_pLevel->IncreaseSubLevel();
					else m_pLevel->DecreaseSubLevel();
					m_Position = door.outcomePos;

					m_InhaledEnemy = false;
					m_pInhaledEnemy = nullptr;

					return true;
				}
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
	m_AbilityType = AbilityType::None;
	m_CurrentState = State::None;
	m_AccumSec = 0;
	m_InvincibleAccumSec = 0;
}

void Kirby::HitEnemy(const Point2f& enemyPos)
{
	if (!m_IsInvincible)
	{
		--m_Health;
		//m_IsInvincible = true;

		Direction hitDirection{ static_cast<Direction>(utils::GetSign(enemyPos.x - m_Position.x)) };
		m_Velocity.x = -static_cast<int>(hitDirection) * 100.f;

		// No animation working yet?
		m_CurrentState = State::Jump;
		m_CurrentAnimation = "Flip";
	}

	// TODO: knockback, also check friction
}
void Kirby::InhaledEnemy(Enemy* enemy)
{
	m_pInhaledEnemy = enemy;
	m_InhaledEnemy = true;
	m_CurrentState = State::None;
	m_Score += enemy->GetScore();
}

void Kirby::AbilityUpdate(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
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
}
void Kirby::Death()
{
	--m_Lives;
	m_Health = 6;
	if (m_Lives <= 0)
	{
		Reset();
	}
}
void Kirby::Invincibility(float elapsedSec)
{
	m_InvincibleAccumSec += elapsedSec;
	if (m_InvincibleAccumSec >= m_INVINCIBILITY_TIME)
	{
		m_InvincibleAccumSec = 0;
		m_IsInvincible = false;
	}
}
void Kirby::UserInput()
{
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
Kirby::Card Kirby::GetCard() const
{
	return m_Card;
}
bool Kirby::IsInvincible() const
{
	return m_IsInvincible;
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
void Kirby::Animate()
{
	if (m_CurrentState != m_OldState)
	{
		m_OldState = m_CurrentState;
		m_AccumSec = 0;
	}

	// Play the correct animation according to Kirby's state
	switch (m_CurrentState)
	{
	case Kirby::State::None:
		if (m_IsOn30)
		{
			if (!m_InhaledEnemy)
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "Idle30Down";
				else m_CurrentAnimation = "Idle30Up";
			}
			else
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "InhaledIdle30Down";
				else m_CurrentAnimation = "InhaledIdle30Up";
			}
		}
		else if (m_IsOn45)
		{
			if (!m_InhaledEnemy)
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "Idle45Down";
				else m_CurrentAnimation = "Idle45Up";
			}
			else
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "InhaledIdle45Down";
				else m_CurrentAnimation = "InhaledIdle45Up";
			}
		}
		else
		{
			if (!m_InhaledEnemy) m_CurrentAnimation = "Idle";
			else m_CurrentAnimation = "InhaledIdle";
		}
		break;
	case Kirby::State::Walk:
		if (m_IsOn45)
		{
			if (m_IsSlopeDown) m_CurrentAnimation = "Walk45Down";
			else m_CurrentAnimation = "Walk45Up";
		}
		else
		{
			if (!m_InhaledEnemy) m_CurrentAnimation = "Walk";
			else m_CurrentAnimation = "InhaledWalk";
		}
		break;
	case Kirby::State::Slide:
		if (abs(m_Velocity.x) > m_WALK_SPEED) m_CurrentAnimation = "Slide";
		else
		{
			if (m_IsOn30)
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "Duck30Down";
				else m_CurrentAnimation = "Duck30Up";
			}
			else if (m_IsOn45)
			{
				if (m_IsSlopeDown) m_CurrentAnimation = "Duck45Down";
				else m_CurrentAnimation = "Duck45Up";
			}
			else m_CurrentAnimation = "Duck";
		}
		break;
	case Kirby::State::Jump:
		if (!m_InhaledEnemy)
		{
			if (m_Velocity.y >= 0.f) m_CurrentAnimation = "Jump";
			else m_CurrentAnimation = "Flip";
		}
		else m_CurrentAnimation = "InhaledJump";
		break;
	case Kirby::State::Falling:
		m_CurrentAnimation = "Falling";
		break;
	case Kirby::State::Flight:
		if (!m_pAnimationManager->IsActive("FlightStart") or m_pAnimationManager->IsDone("FlightStart"))
		{
			m_CurrentAnimation = "Flight";
			if (m_Velocity.y <= 0.f)
			{
				m_CurrentAnimation = "FlightSlow";
			}
		}
		break;
	case Kirby::State::Inhaling:
		m_CurrentAnimation = "Inhaling";
		break;
	case Kirby::State::Exhaling:
		if (m_pAnimationManager->IsDone("Exhaling"))
		{
			m_CurrentState = State::None;
			m_pInhaledEnemy = nullptr;
			m_InhaledEnemy = false;
		}
		break;
	case Kirby::State::Swallow:
		if (m_pAnimationManager->IsDone("Swallow"))
		{
			m_CurrentState = State::None;
			m_pInhaledEnemy = nullptr;
			m_InhaledEnemy = false;
		}
		break;
	case Kirby::State::Ability:
		if(m_AbilityType == AbilityType::Beam) m_CurrentAnimation = "Beam";
		if(m_AbilityType == AbilityType::Fire) m_CurrentAnimation = "Fire";
		if(m_AbilityType == AbilityType::Spark) m_CurrentAnimation = "Spark";
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region CanXxWithCurrentState
bool Kirby::CanMoveWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Swallow or
		m_CurrentState == State::Falling or
		m_CurrentState == State::EnterDoor or
		m_CurrentState == State::Inhaling) return false;
	else return true;
}
bool Kirby::CanJumpWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Inhaling	or
		m_CurrentState == State::Falling	or
		m_CurrentState == State::EnterDoor	or
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
		m_CurrentState == State::EnterDoor or
		m_InhaledEnemy) return false;
	else return true;
}
bool Kirby::CanInhaleWithCurrentState() const
{
	if (m_CurrentState == State::Slide or
		m_CurrentState == State::Flight or
		m_CurrentState == State::Exhaling or
		m_CurrentState == State::Swallow or
		m_CurrentState == State::EnterDoor or
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
		<< "Invincible: " << std::boolalpha << kirby.m_IsInvincible << std::endl << std::endl << std::endl;

	return out;
}
