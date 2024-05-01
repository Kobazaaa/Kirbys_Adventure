#include "pch.h"
#include <iostream>
#include "Entity.h"
#include "TextureManager.h"

const float Entity::m_GRAVITY{ -300.f };

Entity::Entity(const std::string& textureName, float width, float height, const Point2f& center)
	: m_CurrentFrame	{ 0 }
	, m_CurrentFrameRow	{ 0 }
	, m_Width			{ width }
	, m_Height			{ height }
	, m_AccumSec		{ 0 }
	, m_Position		{ center }
	, m_Direction		{ Direction::Right }
	, m_Velocity		{ 30.f, 0.f }
	, m_HitInfo			{}
	, m_SrcRectStart	{ 0, 0 }
	, m_IsInvincible	{ false }
	, m_pSpriteSheet	{ TextureManager::GetTexture(textureName) }
	, m_pAbility		{ nullptr }
{
}

Entity::~Entity()
{
	delete m_pAbility;
	m_pAbility = nullptr;
}

void Entity::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	m_AccumSec += elapsedSec;
	ApplyGravity(elapsedSec);
}

void Entity::Draw(bool flipSprite) const
{
	if (m_pAbility != nullptr)
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
		m_pSpriteSheet->Draw(GetDstRect(), GetSrcRect());
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

Rectf Entity::GetDstRect() const
{
	const Rectf dstRect
	{
		GetDstCenter().x - m_Width  / 2,
		GetDstCenter().y - m_Height / 2,
		m_Width,
		m_Height,
	};
	return dstRect;
}
Point2f Entity::GetDstCenter() const
{
	Point2f dstCenter
	{
		GetHitBox().left + GetHitBox().width / 2,
		GetHitBox().bottom + m_Height / 2
	};

	return dstCenter;
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
	hitBox.width	= 14.f;
	hitBox.height	= 14.f;
	hitBox.left		= m_Position.x - hitBox.width / 2;
	hitBox.bottom	= m_Position.y - hitBox.height / 2;

	return hitBox;
}
const utils::HitInfo& Entity::GetHitInfo() const
{
	return m_HitInfo;
}
Direction Entity::GetDirection() const
{
	return m_Direction;
}
Ability* Entity::GetAbility() const
{
	return m_pAbility;
}
#pragma endregion

#pragma region Update
Rectf Entity::GetSrcRect() const
{
	Rectf srcRect
	{
		m_SrcRectStart.x + m_CurrentFrame * m_Width,
		m_SrcRectStart.y - m_CurrentFrameRow * m_Height,
		m_Width, m_Height
	};
	
	return srcRect;
}
void Entity::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += m_GRAVITY * elapsedSec;
	if (m_Velocity.y <= m_GRAVITY) m_Velocity.y = m_GRAVITY;
	m_Position.y += m_Velocity.y * elapsedSec;
}
#pragma endregion