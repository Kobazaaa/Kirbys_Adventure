#include "pch.h"
#include <iostream>
#include "Entity.h"

const float Entity::m_GRAVITY{ -300.f };

Entity::Entity(const std::string& spriteFilePath, float width, float height, const Point2f& center)
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
	, m_IsEliminated	{ false }
{
	m_pSpriteSheet = new Texture(spriteFilePath);
}

Entity::~Entity()
{
	delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;
}

void Entity::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	m_AccumSec += elapsedSec;
	UpdateSourceRect();
	ApplyGravity(elapsedSec);
}

void Entity::Draw(bool flipSprite) const
{
	glPushMatrix();
	{
		if (flipSprite)
		{
			glTranslatef(m_Position.x, 0, 0);
			glRotatef(-180, 0, 1, 0);
			glTranslatef(-m_Position.x, 0, 0);
		}
		m_pSpriteSheet->Draw(GetDstRect(), m_SrcRect);
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
void Entity::IsEliminated(bool isEliminated)
{
	m_IsEliminated = isEliminated;
}
void Entity::InverseDirection()
{
	m_Direction = static_cast<Entity::Direction>(-static_cast<int>(m_Direction));
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
	hitBox.width	= 16.f;
	hitBox.height	= 16.f;
	hitBox.left		= m_Position.x - hitBox.width / 2;
	hitBox.bottom	= m_Position.y - hitBox.height / 2;

	return hitBox;
}
utils::HitInfo& Entity::GetHitInfo()
{
	return m_HitInfo;
}
Entity::Direction Entity::GetDirection() const
{
	return m_Direction;
}
#pragma endregion

#pragma region Update
void Entity::UpdateSourceRect()
{
	m_SrcRect.left		= m_SrcRectStart.x + m_CurrentFrame		* m_Width;
	m_SrcRect.bottom	= m_SrcRectStart.y - m_CurrentFrameRow  * m_Height;
	m_SrcRect.width		= m_Width;
	m_SrcRect.height	= m_Height;
}
void Entity::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += m_GRAVITY * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
}
#pragma endregion