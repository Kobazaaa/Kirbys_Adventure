#include "pch.h"
#include "utils.h"
#include "Kirby.h"
#include "Level.h"
#include "ViewFade.h"
#include "Camera.h"
#include <iostream>
#include "ParticleSystem.h"

using namespace utils;

Kirby::Kirby( const Point2f& center)
	: Entity("Kirby2", 16, 16, center, 0)
	, m_Health						{ 6 }
	, m_Lives						{ 4 }
	, m_HasInhaledAbility			{ false }
	, m_Puff						{ }
	, m_StarProj					{ }
	, m_Star						{ nullptr }
	, m_InhaledAbilityType			{ AbilityType::None }
	, m_Card						{ Card::Ability }
	, m_InvincibleAccumSec			{ 0 }
	, m_AbilityActivationAccumSec	{ 0 }
	, m_SavePoint					{ center }
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
	case Kirby::State::Duck:
		enumAsString = "Duck";
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
	case Kirby::State::Ability:
		enumAsString = "Ability";
		break;
	case Kirby::State::EnterDoor:
		enumAsString = "EnterDoor";
		break;
	case Kirby::State::Land:
		enumAsString = "Land";
		break;
	case Kirby::State::Dead:
		enumAsString = "Dead";
		break;
	default:
		enumAsString = "State not found";
		break;
	}

	return enumAsString;
}

void Kirby::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	Entity::Update(elapsedSec, world);
	Animate();

	if (m_DoesWorldCollision) Collisions(world);


	if (m_CurrentState != State::Dead)
	{
		m_DoesWorldCollision = true;

		if (m_Card != Card::Ability)
		{
			m_CardAccumSec += elapsedSec;
			if (m_CardAccumSec >= 1.f)
			{
				m_CardAccumSec = 0;
				m_Card = Card::Ability;
			}
		}
		AbilityUpdate(elapsedSec, world);

		if (m_IsInvincible) Invincibility(elapsedSec);
		SlopeHandling();
		if (m_IsUnderwater)
		{
			ApplyUnderwaterChanges();
			if (m_AccumSec >= 2.0f)
			{
				ParticleSystem::AddAirBubbles(m_Position);
				m_AccumSec = 0;
			}
		}
		else if(m_WasInWater)
		{
			m_WasInWater = false;
			if (m_CurrentState != State::Flight and m_CurrentState != State::Dead)
			{
				m_Velocity.y = m_JUMP_SPEED;
				m_CurrentState = State::Jump;
			}
		}

		MovementUpdate(elapsedSec);
		MechanicUpdate(elapsedSec);

	}


	if (m_Health <= 0) Death();
	else if (m_Health <= 1 and m_AccumSec >= 30.f)
	{
		m_AccumSec = 0;
		SoundManager::PlayEffect("LowHealth");
	}


	if (utils::DEBUG_MODE)
	{
		if (KeyPress(SDL_SCANCODE_I)) std::cout << *this << std::endl;
		if (KeyPress(SDL_SCANCODE_R)) --m_Health;
		if (KeyPress(SDL_SCANCODE_K)) --m_Lives;
	}
}
void Kirby::Draw() const
{
	m_Puff.Draw();
	m_StarProj.Draw();
	m_Star.Draw();

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
		if ((KeyDown(SDL_SCANCODE_DOWN) and CanSlideWithCurrentState()) and !m_IsSliding and !m_IsUnderwater)
		{
			m_CurrentState = State::Duck;
			if (m_OldState != State::Duck) SoundManager::PlayEffect("Duck");

		}
		if (m_CurrentState == State::Duck and KeyPress(SDL_SCANCODE_SPACE) and CanSlideWithCurrentState() and !m_IsSliding)
		{
			m_CurrentState = State::Slide;
			if (m_OldState != State::Slide) SoundManager::PlayEffect("Slide");
			m_AccumSec = 0;
			m_IsSliding = true;
		}
		if (m_IsSliding)
		{
			//ParticleSystem::AddRunParticles(Point2f(m_Position.x, m_Position.y - 8), Vector2f(-static_cast<int>(m_Direction) * 50, 0));

			m_CurrentState = State::Slide;
			if (m_AccumSec >= m_SLIDING_TIME)
			{
				m_Velocity.x = static_cast<int>(m_Direction) * 3.f;
				m_IsSliding = false;
				m_AccumSec = 0;
			}
			else m_Velocity.x = static_cast<int>(m_Direction) * m_SLIDE_SPEED;
		}
#pragma endregion

#pragma region DirectionalMovement
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~	   RUNNING & TURNING	~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ((m_CurrentState == State::None or m_CurrentState == State::Walk) and !m_IsUnderwater)
		{
			if (KeyPress(SDL_SCANCODE_RIGHT) and abs(m_Velocity.x) >= 5.f)
			{
				if (m_Direction == Direction::Left and !m_HasInhaledAbility)
				{
					m_IsTurning = true;
					SoundManager::PlayEffect("Turn");
					ParticleSystem::AddRunParticles(Point2f(m_Position.x + 8, m_Position.y - 8), Vector2f(10, 0));
				}
				else m_IsTurning = false;
				if (m_Direction == Direction::Right and !m_IsTurning and !m_IsRunning)
				{
					ParticleSystem::AddRunParticles(Point2f(m_Position.x, m_Position.y - 8), Vector2f(-50, 0));
					m_IsRunning = true;
				}
				else m_IsRunning = false;
			}
			if (KeyPress(SDL_SCANCODE_LEFT) and abs(m_Velocity.x) >= 5.f)
			{
				if (m_Direction == Direction::Right and !m_HasInhaledAbility)
				{
					m_IsTurning = true;
					SoundManager::PlayEffect("Turn");
					ParticleSystem::AddRunParticles(Point2f(m_Position.x - 8, m_Position.y - 8), Vector2f(-10, 0));
				}
				else m_IsTurning = false;
				if (m_Direction == Direction::Left and !m_IsTurning and !m_IsRunning)
				{
					m_IsRunning = true;
					ParticleSystem::AddRunParticles(Point2f(m_Position.x, m_Position.y - 8), Vector2f(50, 0));
				}
				else m_IsRunning = false;
			}
		}
		else if (m_CurrentState != State::Jump and m_CurrentState != State::Land) m_IsRunning = false;

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
		if (m_IsUnderwater and KeyDown(SDL_SCANCODE_SPACE))
		{
			m_Velocity.y = m_SWIM_SPEED;
		}
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
		if (KeyRelease(SDL_SCANCODE_SPACE) and m_CurrentState != State::Flight)
		{
			if (m_Velocity.y >= 0) m_Velocity.y = 0;
		}
#pragma endregion

#pragma region Flight
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ~~			FLIGHT			~~
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (m_IsGrounded and m_CurrentState == State::Flight) m_Position.y += 1;
		if (m_IsUnderwater and m_CurrentState == State::Flight) m_Velocity.y = 30.f;

		if ((KeyDown(SDL_SCANCODE_UP) and CanFlyWithCurrentState()) or (KeyDown(SDL_SCANCODE_SPACE) and m_CurrentState == State::Flight))
		{
 			if (m_CurrentState != State::EnterDoor and !m_IsUnderwater)
			{
				if (m_CurrentState != State::Flight)
				{
					m_CurrentAnimation = "FlightStart";
					SoundManager::PlayEffect("StartFlight");
				}
				m_CurrentState = State::Flight;
				m_Velocity.y = m_FLIGHT_SPEED;
			}
		}
		if (m_CurrentState == State::Flight) m_GravityMultiplier = 0.5f;
		else if (!m_IsUnderwater) m_GravityMultiplier = 1.f;
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
		if (m_HasInhaledAbility)
		{
			m_CurrentAnimation = "Exhaling";
			m_CurrentState = State::Exhaling;
			if (!m_StarProj.IsActivated())
			{
				SoundManager::PlayEffect("StarSpit");
				m_StarProj.Activate(Point2f(m_Position.x, m_Position.y + 4), m_Direction);
				m_HasInhaledAbility = false;
				m_InhaledAbilityType = AbilityType::None;
			}
		}
		else if (m_CurrentState == State::Flight)
		{
			m_CurrentAnimation = "Exhaling";
			m_CurrentState = State::Exhaling;
			if (!m_Puff.IsActivated())
			{
				m_Puff.Activate(m_Position, m_Direction);
				SoundManager::PlayEffect("Puff");
			}
		}
		else if (m_AbilityType != AbilityType::None)
		{
			if (!m_pAbility->IsActive() and !m_IsUnderwater) m_pAbility->Activate(m_Position, m_Direction);
		}
		else if (CanInhaleWithCurrentState() and !m_IsUnderwater)
		{
			SoundManager::PlayEffect("InhalingStart");
			m_CurrentState = State::Inhaling;
			m_AccumSec = 0;
		}
	}

	if (m_CurrentState == State::Inhaling)
	{
		ParticleSystem::AddInhaleParticles(m_Position, GetInhaleRect());

		if (utils::IsOverlapping(m_Star.GetHitBox(), GetInhaleRect()))
		{
			float xInc{ -elapsedSec * 10.f * (m_Star.GetPosition().x - GetPosition().x) };
			float yInc{ -elapsedSec * 10.f * ((m_Star.GetPosition().y - (GetInhaleRect().bottom + (GetInhaleRect().height / 2)))) };

			Point2f newStarPos
			{
				m_Star.GetPosition().x + xInc,
				m_Star.GetPosition().y + yInc
			};

			m_Star.SetPosition(newStarPos);
		}

		if (!m_HasInhaledAbility and utils::IsOverlapping(m_Star.GetHitBox(), GetHitBox()) and m_Star.IsActivated())
		{
			m_HasInhaledAbility = true;
			m_InhaledAbilityType = m_Star.GetAbility();
			m_Star.Deactivate();
			m_CurrentState = State::None;
		}

		if (!SoundManager::IsPlayingEffect("InhalingStart") and !SoundManager::IsPlayingEffect("Inhaling"))
		{
			SoundManager::PlayEffect("Inhaling");
		}
	}
	else
	{
		if (SoundManager::IsPlayingEffect("InhalingStart")) SoundManager::StopEffect("InhalingStart");
		if (SoundManager::IsPlayingEffect("Inhaling")) SoundManager::StopEffect("Inhaling");
	}
	if (!m_HasInhaledAbility) m_InhaledAbilityType = AbilityType::None;

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
			m_AccumSec = 0;
		}
	}
#pragma endregion

#pragma region RemoveAbility
	if (KeyPress(SDL_SCANCODE_RSHIFT) and m_AbilityType != AbilityType::None and !m_pAbility->IsActive() and m_CurrentState != State::Swallow)
	{
		m_Star.Activate(m_Position, static_cast<Direction>(-static_cast<int>(m_Direction)));
		m_Star.SetAbility(m_AbilityType);

		delete m_pAbility;
		m_pAbility = nullptr;
		m_AbilityType = AbilityType::None;
	}
#pragma endregion

#pragma region Swallow
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			SWALLOW			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (m_CurrentState == State::Swallow and m_AbilityType != AbilityType::None)
	{
		ViewFade::Darken(.5f);
		ParticleSystem::AddAbilityActivation(m_Position);
	}
	else if (m_CurrentState != State::Swallow and m_AbilityType != AbilityType::None and !m_pAbility->IsActive()) ViewFade::RemoveDarken();

	if (m_HasInhaledAbility and (m_CurrentState == State::None or m_CurrentState == State::Walk) and KeyPress(SDL_SCANCODE_DOWN) and m_IsGrounded)
	{
		m_HasInhaledAbility = false;
		m_CurrentAnimation = "Swallow";
		m_CurrentState = State::Swallow;
		delete m_pAbility;

		if (m_InhaledAbilityType != AbilityType::None)
		{
			if (m_InhaledAbilityType == AbilityType::Beam)		m_pAbility = new Beam(true);
			if (m_InhaledAbilityType == AbilityType::Fire)		m_pAbility = new Fire(true);
			if (m_InhaledAbilityType == AbilityType::Spark)		m_pAbility = new Spark(true);
			m_AbilityType = m_InhaledAbilityType;

			m_Card = Card::Ability;

			SoundManager::PlayEffect("PowerUpReceived");
			m_InhaledAbilityType = AbilityType::None;
		}
		else
		{
			m_Card = Card::Nothing;
			SoundManager::PlayEffect("NoPowerUpReceived");
		}
	}
#pragma endregion
}
void Kirby::Collisions(const std::vector<std::vector<Point2f>>& world)
{
	if (Collision::WallCollision(this, world))
	{
		m_IsSliding = false;

		//TODO fix wall animation
		if (m_CurrentState == State::Walk and !m_HasInhaledAbility)
		{
			m_CurrentAnimation = "Wall";
			if (!m_pAnimationManager->IsActive("Wall") or m_pAnimationManager->IsDone("Wall"))
			{
				//m_CurrentState = State::None;
				m_CurrentAnimation = "Idle";
			}
		}
		m_Velocity.x = 0;
	}

	m_IsOn45 = false;
	m_IsOn30 = false;
	if (Collision::FloorCollision(this, world))
	{
		m_IsGrounded = true;

		if (m_CurrentState != State::Exhaling and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Falling and
			m_CurrentState != State::Swallow and
			m_CurrentState != State::Land and
			m_CurrentState != State::EnterDoor and
			!m_IsHit and
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
				SoundManager::PlayEffect("Falling");
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
		
		if (m_WasInAir)
		{
			m_WasInAir = false;
			if (CanJumpWithCurrentState() and !m_HasInhaledAbility and !m_IsUnderwater and ((m_AbilityType != AbilityType::None and !m_pAbility->IsActive()) or m_AbilityType == AbilityType::None))
			{
				ParticleSystem::AddLandParticles(m_Position);
				SoundManager::PlayEffect("Land");
				m_CurrentState = State::Land;
			}
		}
	}
	else
	{
		m_WasInAir = true;
		m_IsGrounded = false;

		if (m_Velocity.y < m_GRAVITY / 1.75f and (m_CurrentState == State::None or m_CurrentState == State::Jump or m_CurrentState == State::Falling or m_CurrentState == State::Walk) and !m_HasInhaledAbility)
		{
			if (m_pAbility == nullptr or !m_pAbility->IsActive())
			{
				if (!m_IsUnderwater) m_CurrentState = State::Falling;
			}
		}
		if (m_CurrentState == State::Falling and m_IsUnderwater) m_CurrentState = State::None;

		if (m_Velocity.y < 0.f and
			m_CurrentState != State::Flight and
			m_CurrentState != State::Exhaling and
			m_CurrentState != State::Inhaling and
			m_CurrentState != State::Jump and
			m_CurrentState != State::Falling and
			m_CurrentState != State::EnterDoor and
			m_CurrentState != State::Dead and
			!m_IsHit and
			!m_IsUnderwater and
			(m_AbilityType == AbilityType::None or !m_pAbility->IsActive()))
		{
			m_CurrentState = State::None;
			if (!m_HasInhaledAbility) m_CurrentAnimation = "EndFlip";
			else m_CurrentAnimation = "InhaledJumpEnd";
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
void Kirby::ApplyPlaySpace(Level* pLevel)
{
	if (m_DoesWorldCollision)
	{
		const Rectf subLevel
		{
			0,
			pLevel->GetCurrentSubLevel() * pLevel->GetSubLevelHeight(),
			pLevel->GetWidth(),
			pLevel->GetSubLevelHeight()
		};

		if (m_Position.x - GetHitBox().width / 2 < subLevel.left)							m_Position.x = subLevel.left + GetHitBox().width / 2;
		if (m_Position.x + GetHitBox().width / 2 > subLevel.left + subLevel.width)			m_Position.x = subLevel.left + subLevel.width - GetHitBox().width / 2;
		if (m_Position.y + GetHitBox().height / 2 > subLevel.bottom + subLevel.height)
		{
			m_Position.y = subLevel.bottom + subLevel.height - GetHitBox().height / 2;
			m_Velocity.y = 0;
		}
		if (m_Position.y + GetHitBox().height / 2 < subLevel.bottom) m_Health = 0;
	}
	m_Star.ApplyPlaySpace(pLevel);
}

bool Kirby::HasEnteredDoor()
{
	if (m_CurrentState == State::EnterDoor)
	{
		m_Velocity.x = 0;
		m_Velocity.y = 0;

		if (m_pAnimationManager->IsDone("Exhaling"))
		{
			m_CurrentAnimation = "EnterDoor";
		}
		if (m_pAnimationManager->IsDone("EnterDoor"))
		{
			SoundManager::PlayEffect("DoorEnter");

			m_Direction = Direction::Right;
			m_CurrentState = State::None;
			m_HasInhaledAbility = false;
			m_InhaledAbilityType = AbilityType::None;
			return true;
		}
	}

	return false;
}

void Kirby::EnterDoor()
{
	if (utils::KeyPress(SDL_SCANCODE_UP))
	{
		m_CurrentState = State::EnterDoor;
		m_Velocity.x = 0;
		if (m_OldState != State::Flight and !m_HasInhaledAbility)
		{
			m_Direction = Direction::Right;
			m_CurrentAnimation = "EnterDoor";
		}
		else
		{
			m_CurrentAnimation = "Exhaling";
			if (m_HasInhaledAbility)
			{
				if (!m_StarProj.IsActivated())
				{
					m_StarProj.Activate(m_Position, m_Direction);
					SoundManager::PlayEffect("StarSpit");
				}
			}
			else if (m_OldState == State::Flight)
			{
				if (!m_Puff.IsActivated())
				{
					m_Puff.Activate(m_Position, m_Direction);
					SoundManager::PlayEffect("Puff");
				}
			}
		}
	}

}

void Kirby::ForceInhale()
{
	m_CurrentState = State::Inhaling;
}

void Kirby::Reset()
{
	m_GameOver = false;
	m_WasInWater = false;
	m_IsUnderwater = false;
	m_DeathFade = true;
	m_IsInvincible = false;
	m_HasInhaledAbility = false;
	m_InhaledAbilityType = AbilityType::None;
	delete m_pAbility;
	m_pAbility = nullptr;
	m_AbilityType = AbilityType::None;
	m_CurrentState = State::None;
	m_CurrentAnimation = "Idle";
	m_AccumSec = 0;
	m_InvincibleAccumSec = 0;

	m_WalkSpeedMultiplier = 1.f;
	m_GravityMultiplier = 1.f;
}

void Kirby::HardReset()
{
	Reset();
	m_IsHit = false;
	m_IsSliding = false;
	m_IsRunning = false;
	m_Velocity = Vector2f(0, 0);
	m_Score = 0;
	m_Health = 6;
	m_Lives = 4;
}

void Kirby::HitEnemy(const Point2f& enemyPos)
{
	if (!m_IsInvincible)
	{
		if (!m_IsSliding and m_CurrentState != State::Falling)
		{
			--m_Health;
			m_IsInvincible = true;

			m_Card = Card::Damaged;

			Direction hitDirection{ static_cast<Direction>(utils::GetSign(enemyPos.x - m_Position.x)) };
			m_Velocity.x = -static_cast<int>(hitDirection) * 100.f;

			if (m_AbilityType != AbilityType::None)
			{
				m_Star.Activate(m_Position, static_cast<Direction>(-static_cast<int>(m_Direction)));
				m_Star.SetAbility(m_AbilityType);

				m_AbilityType = AbilityType::None;
				delete m_pAbility;
				m_pAbility = nullptr;
			}

			SoundManager::PlayEffect("Hit");
			m_IsHit = true;
		}
		else
		{
			SoundManager::PlayEffect("EnemyDeath");
		}
	}
}
void Kirby::InhaledEnemy(AbilityType ability)
{
	m_HasInhaledAbility = true;
	m_InhaledAbilityType = ability;
	m_CurrentState = State::None;
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
	m_Star.Update(elapsedSec, world);
}
void Kirby::Death()
{
	if (m_CurrentState != State::Dead)
	{
		m_WalkSpeedMultiplier = 1.f;
		m_GravityMultiplier = 1.f;
		
		m_AccumSec = 0;
		m_Velocity.x = 0;
		m_Velocity.y = m_JUMP_SPEED;
		m_Position.y += 1;
		m_DoesWorldCollision = false;
		SoundManager::PlayEffect("Dead");
		ParticleSystem::AddKirbyDeathParticles(m_Position);
	}
	m_CurrentState = State::Dead;

	if (m_AccumSec >= 3.f)
	{
		m_CurrentAnimation = "Idle";
		if (m_DeathFade) ViewFade::StartFade(0.5f, false);
		m_DeathFade = false;

		if (ViewFade::IsFadingIn())
		{
			if (!m_GameOver and m_CurrentState == State::Dead)
			{
				m_Health = 6;
				--m_Lives;
				m_Position = m_SavePoint;
				Reset();
				if (m_Lives < 0)
				{
					m_GameOver = true;
				}
			}
		}

	}
	else
	{
		SoundManager::ResetStream();
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

void Kirby::ApplyUnderwaterChanges()
{
	m_WalkSpeedMultiplier = 0.5f;
	m_GravityMultiplier = 0.2f;

	m_WasInWater = true;
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
bool Kirby::IsGameOver() const
{
	return m_GameOver;
}
void Kirby::AddToScore(int addition)
{
	m_Score += addition;
}
void Kirby::SetHealth(int health)
{
	m_Health = health;
}
void Kirby::SetSavePoint(const Point2f& pos)
{
	m_SavePoint = pos;
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

	// Play the correct animation according to Kirby's state
	switch (m_CurrentState)
	{
	case Kirby::State::None:
		if (m_IsOn30)
		{
			if (!m_HasInhaledAbility)
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
			if (!m_HasInhaledAbility)
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
			if (!m_HasInhaledAbility) m_CurrentAnimation = "Idle";
			else m_CurrentAnimation = "InhaledIdle";
		}
		break;
	case Kirby::State::Walk:
		if (m_IsOn45 and !m_HasInhaledAbility)
		{
			if (m_IsSlopeDown) m_CurrentAnimation = "Walk45Down";
			else m_CurrentAnimation = "Walk45Up";
		}
		else
		{
			if (m_IsRunning)
			{
				if (!m_HasInhaledAbility) m_CurrentAnimation = "Run";
				else m_CurrentAnimation = "InhaledRun";
			}
			else
			{
				if (!m_HasInhaledAbility) m_CurrentAnimation = "Walk";
				else m_CurrentAnimation = "InhaledWalk";
			}
		}
		break;
	case Kirby::State::Slide:
		if (abs(m_Velocity.x) > m_RUN_SPEED)
		{
			m_CurrentAnimation = "Slide";
		}
		break;
	case Kirby::State::Duck:
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
		if (!m_HasInhaledAbility)
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
			m_InhaledAbilityType = AbilityType::None;
			m_HasInhaledAbility = false;
		}
		break;
	case Kirby::State::Swallow:
		if (m_pAnimationManager->IsDone("Swallow"))
		{
			m_CurrentState = State::None;
			m_InhaledAbilityType = AbilityType::None;
			m_HasInhaledAbility = false;
			if(m_AbilityType != AbilityType::None) m_pAbility->Activate(m_Position, m_Direction);
		}
		break;
	case Kirby::State::Ability:
		if(m_AbilityType == AbilityType::Beam) m_CurrentAnimation = "Beam";
		if(m_AbilityType == AbilityType::Fire) m_CurrentAnimation = "Fire";
		if(m_AbilityType == AbilityType::Spark) m_CurrentAnimation = "Spark";
		break;
	case Kirby::State::Land:
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
		if (m_pAnimationManager->IsDone("Duck30Down") or
			m_pAnimationManager->IsDone("Duck30Up") or
			m_pAnimationManager->IsDone("Duck45Down") or
			m_pAnimationManager->IsDone("Duck45Up") or
			m_pAnimationManager->IsDone("Duck"))
		{
			m_CurrentState = State::None;
		}
		break;
	case Kirby::State::Dead:
		m_CurrentAnimation = "Death";
		break;
	default:
		break;
	}

	if (m_IsUnderwater and m_CurrentState != State::Flight and m_CurrentState != State::Dead)
	{
		if (!m_IsGrounded and m_Velocity.y >= m_SWIM_SPEED - 5.f and abs(m_Velocity.x) < 5.f)
		{
			if (!m_HasInhaledAbility) m_CurrentAnimation = "SwimUp";
			else m_CurrentAnimation = "InhaledWalk";
		}
		else if (!m_IsGrounded and abs(m_Velocity.x) > 5.f)
		{
			if (!m_HasInhaledAbility) m_CurrentAnimation = "SwimSide";
			else m_CurrentAnimation = "InhaledWalk";
		}
		else if (!m_IsGrounded)
		{
			m_CurrentState = State::None;
			if (!m_HasInhaledAbility) m_CurrentAnimation = "EndFlip";
			else m_CurrentAnimation = "InhaledJumpEnd";
		}
	}

	if (m_IsHit and m_CurrentState != State::Dead)
	{
		if (!m_HasInhaledAbility)
		{
			m_CurrentAnimation = "Roll";
			if (m_pAnimationManager->IsDone("Roll"))
			{
				m_IsHit = false;
				m_CurrentState = State::None;
				m_InhaledAbilityType = AbilityType::None;
				m_HasInhaledAbility = false;
			}
		}
		if (m_HasInhaledAbility or m_CurrentState == State::Flight)
		{
			m_CurrentAnimation = "InhaledRoll";
			if (m_pAnimationManager->IsDone("InhaledRoll"))
			{
				m_IsHit = false;
				m_CurrentState = m_OldState;
			}
		}

	}


	if (m_CurrentState != m_OldState)
	{
		m_OldState = m_CurrentState;
	}

}
#pragma endregion

#pragma region CanXxWithCurrentState
bool Kirby::CanMoveWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Duck		or
		m_CurrentState == State::Swallow	or
		m_CurrentState == State::Falling	or
		m_CurrentState == State::EnterDoor	or
		m_IsHit								or
		m_CurrentState == State::Inhaling) return false;
	else return true;
}
bool Kirby::CanJumpWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Duck		or
		m_CurrentState == State::Inhaling	or
		m_CurrentState == State::Falling	or
		m_CurrentState == State::EnterDoor	or
		m_CurrentState == State::Jump		or
		m_CurrentState == State::Flight		or
		m_CurrentState == State::Exhaling	or
		m_CurrentState == State::Ability	or
		m_CurrentState == State::Swallow	or
		m_IsHit								or
		!m_IsGrounded						or
		m_Velocity.y != 0.f) return false;
	else return true;
}
bool Kirby::CanSlideWithCurrentState() const
{
	if (m_HasInhaledAbility) return false;
	if (m_CurrentState == State::None or
		m_CurrentState == State::Duck or
		m_CurrentState == State::Slide) return true;
	else return false;
}
bool Kirby::CanFlyWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Duck		or
		m_CurrentState == State::Exhaling	or
		m_CurrentState == State::Swallow	or
		m_CurrentState == State::EnterDoor	or
		m_IsHit								or
		m_HasInhaledAbility) return false;
	else return true;
}
bool Kirby::CanInhaleWithCurrentState() const
{
	if (m_CurrentState == State::Slide		or
		m_CurrentState == State::Duck		or
		m_CurrentState == State::Flight		or
		m_CurrentState == State::Exhaling	or
		m_CurrentState == State::Swallow	or
		m_CurrentState == State::EnterDoor	or
		m_IsHit								or
		m_HasInhaledAbility) return false;
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
