#include "pch.h"
#include <iostream>
#include "Entity.h"
#include "TextureManager.h"

Entity::Entity(const std::string& textureName, float width, float height, const Point2f& center, int score, bool doesWorldCollsion)
	: m_Width				{ width }
	, m_Height				{ height }
	, m_AccumSec			{ 0 }
	, m_Position			{ center }
	, m_Direction			{ Direction::Right }
	, m_Velocity			{ 0.f, 0.f }
	, m_HitInfo				{ }
	, m_IsInvincible		{ false }
	, m_pAbility			{ nullptr }
	, m_AbilityType			{ AbilityType::None }
	, m_GravityMultiplier	{ 1.f }
	, m_pAnimationManager	{ new AnimationManager(textureName)}
	, m_CurrentAnimation	{ "" }
	, m_Score				{ score }
	, m_DoesWorldCollision	{ doesWorldCollsion }
	, m_IsGrounded			{ false }
	, m_IsUnderwater		{ false }
	, m_WalkSpeedMultiplier	{ 1.f }
{
	m_pAnimationManager->LoadFromFile("Animations/" + textureName + ".xml");
}

Entity::~Entity() noexcept
{
	delete m_pAbility;
	m_pAbility = nullptr;

	delete m_pAnimationManager;
	m_pAnimationManager = nullptr;
}

void Entity::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	m_AccumSec += elapsedSec;
	ApplyGravity(elapsedSec);

	m_pAnimationManager->Update(elapsedSec, m_CurrentAnimation);
}

void Entity::Draw(bool flipSprite) const
{
	if (m_AbilityType != AbilityType::None)
	{
		m_pAbility->Draw();
	}

	glPushMatrix();
	{
		if (flipSprite)
		{
			glTranslatef(m_Position.x, 0, 0);
			glRotatef(-180, 0, 1, 0);
			glTranslatef(-m_Position.x, 0, 0);
		}
		m_pAnimationManager->Draw(m_Position);
	}
	glPopMatrix();
}

#pragma region Mutators
void Entity::SetPosition(const Point2f& newCenterPos)
{
	m_Position = newCenterPos;
}
void Entity::SetPosition(float centerX, float centerY)
{
	SetPosition(Point2f(centerX, centerY));
}
void Entity::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}
void Entity::InverseDirection()
{
	m_Direction = static_cast<Direction>(-static_cast<int>(m_Direction));
}
void Entity::IsUnderwater(bool isUdnerwater)
{
	if (isUdnerwater and !m_IsUnderwater)
	{
		m_Velocity.y = -5.f;
	}
	m_IsUnderwater = isUdnerwater;
}
#pragma endregion

#pragma region Accessors
Point2f Entity::GetPosition() const
{
	return m_Position;
}
Vector2f Entity::GetVelocity() const
{
	return m_Velocity;
}

float Entity::GetWidth() const
{
	return m_Width;
}
float Entity::GetHeight() const
{
	return m_Height;
}
Rectf Entity::GetHitBox() const
{
	Rectf hitBox{};
	hitBox.width	= m_Width;
	hitBox.height	= m_Height;
	hitBox.left		= m_Position.x - hitBox.width / 2;
	hitBox.bottom	= m_Position.y - hitBox.height / 2;

	return hitBox;
}
utils::HitInfo& Entity::GetHitInfo()
{
	return m_HitInfo;
}
Direction Entity::GetDirection() const
{
	return m_Direction;
}
Ability* Entity::GetAbilityPtr() const
{
	return m_pAbility;
}
int Entity::GetScore() const
{
	return m_Score;
}
Entity::AbilityType Entity::GetAbilityType() const
{
	return m_AbilityType;
}
#pragma endregion

#pragma region Update
void Entity::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += (m_GRAVITY * m_GravityMultiplier) * elapsedSec;
	if (m_Velocity.y <= m_GRAVITY) m_Velocity.y = m_GRAVITY;
	m_Position.y += m_Velocity.y * elapsedSec;
}
#pragma endregion