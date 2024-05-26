#include "pch.h"
#include "Kirby.h"
#include "Collision.h"
#include "PowerUp.h"
#include "WorldUtils.h"

PowerUp::PowerUp(const Point2f& position, PowerUp::Type powerUpType)
	: m_Position			{ position }
	, m_Velocity			{ 0, 0 }
	, m_PowerUpType			{ powerUpType }
	, m_IsConsumed			{ false }
	, m_pAnimationManager	{ new AnimationManager("PowerUps")}
{
	m_pAnimationManager->LoadFromFile("Animations/PowerUps.xml");
}

PowerUp::~PowerUp() noexcept
{
	delete m_pAnimationManager;
	m_pAnimationManager = nullptr;
}

void PowerUp::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Kirby* pKirby)
{
	if (!m_IsConsumed)
	{
		m_Velocity.y += m_GRAVITY * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		Animate(elapsedSec, m_PowerUpType);

		Collision::WallCollision(this, world);
		Collision::FloorCollision(this, world);

		if (Collision::PowerUpCollision(pKirby, this))
		{
			this->Consume();
			if (m_PowerUpType == PowerUp::Type::Tomato) pKirby->SetHealth(6);
		}
	}
}

void PowerUp::Draw() const
{
	if (!m_IsConsumed)
	{
		m_pAnimationManager->Draw(m_Position);
	}
}

void PowerUp::Consume()
{
	m_IsConsumed = true;
}

void PowerUp::Respawn()
{
	m_IsConsumed = false;
}

void PowerUp::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

void PowerUp::SetVelocity(const Vector2f& vel)
{
	m_Velocity = vel;
}

Point2f PowerUp::GetPosition() const
{
	return m_Position;
}

Rectf PowerUp::GetHitBox() const
{
	Rectf hitBox{};
	hitBox.width	= 16;
	hitBox.height	= 16;
	hitBox.left		= m_Position.x - hitBox.width  / 2;
	hitBox.bottom	= m_Position.y - hitBox.height / 2;

	return hitBox;
}

const utils::HitInfo& PowerUp::GetHitInfo() const
{
	return m_HitInfo;
}

void PowerUp::Animate(float elapsedSec, PowerUp::Type powerUpType)
{
	switch (powerUpType)
	{
	case PowerUp::Type::Tomato:
		m_pAnimationManager->Update(elapsedSec, "Tomato");
		break;
	case PowerUp::Type::Drink:
		break;
	case PowerUp::Type::Lollipop:
		break;
	case PowerUp::Type::LifeUp:
		break;
	default:
		break;
	}
}
