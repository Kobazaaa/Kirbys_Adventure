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
	, m_SpriteCenter			{ center }
	, m_Direction		{ Direction::Right }
	, m_Velocity		{ 30.f, 0.f }
	, m_HitInfo			{}
	, m_SrcRectStart	{ 0, 0 }
	, m_IsInvincible	{ false }
	, m_IsEliminated	{ false }
	, m_WorldFloorCollision	{ false }
	, m_HitBox			{}
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
	FloorCollisionDetection(world);
}

void Entity::Draw(bool flipSprite) const
{
	glPushMatrix();
	{
		if (flipSprite)
		{
			glTranslatef(m_SpriteCenter.x, 0, 0);
			glRotatef(-180, 0, 1, 0);
			glTranslatef(-m_SpriteCenter.x, 0, 0);
		}
		m_pSpriteSheet->Draw(GetDstRect(), m_SrcRect);
	}
	glPopMatrix();
}

#pragma region Mutators
void Entity::SetPosition(const Point2f& newCenterPos)
{
	m_SpriteCenter = newCenterPos;
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
	return m_SpriteCenter;
}
Vector2f Entity::GetVelocity() const
{
	return m_Velocity;
}
Rectf Entity::GetDstRect() const
{
	const Rectf dstRect
	{
		m_SpriteCenter.x - m_Width  / 2,
		m_SpriteCenter.y - m_Height / 2,
		m_Width,
		m_Height,
	};
	return dstRect;
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
	return m_HitBox;
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
void Entity::FloorCollisionDetection(const std::vector<std::vector<Point2f>>& world)
{
	m_WorldFloorCollision = false;
	if (Collision::FloorCollision(this, world))
	{
		m_WorldFloorCollision = true;
	}
}
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
	m_SpriteCenter.y += m_Velocity.y * elapsedSec;
}
void Entity::UpdateHitBox()
{
	m_HitBox.left = m_SpriteCenter.x - 8.f;
	m_HitBox.bottom = m_SpriteCenter.y - 8.f;
	m_HitBox.width = 16.f;
	m_HitBox.height = 16.f;
}
#pragma endregion