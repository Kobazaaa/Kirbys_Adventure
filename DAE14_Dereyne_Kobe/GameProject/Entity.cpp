#include "pch.h"
#include <iostream>
#include "Entity.h"

Entity::Entity(const std::string& spriteFilePath, float width, float height, const Point2f& center)
	: m_CurrentFrame	{0}
	, m_CurrentFrameRow	{0}
	, m_Width			{width}
	, m_Height			{height}
	, m_AccumSec		{0}
	, m_Center			{center}
	, m_Direction		{ Direction::Right }
	, m_Velocity		{ 30.f, 0.f }
	, m_HitInfo			{}
	, m_SrcRectStart	{0, 0}
	, m_IsInvincible	{ false }
	, m_IsEliminated	{ false }
	, m_WorldCollision	{ false }
	, m_HitBox			{}
{
	m_pSpriteSheet = new Texture(spriteFilePath);
}

Entity::~Entity()
{
	delete m_pSpriteSheet;
}

void Entity::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world)
{
	m_AccumSec += elapsedSec;
	UpdateSourceRect();
	ApplyGravity(elapsedSec);
	FloorCollision(world);
	WallCollision(world);
}

void Entity::Draw(bool flipSprite) const
{
	glPushMatrix();
	{
		if (flipSprite)
		{
			glTranslatef(m_Center.x, 0, 0);
			glRotatef(-180, 0, 1, 0);
			glTranslatef(-m_Center.x, 0, 0);
		}
		m_pSpriteSheet->Draw(GetDstRect(), m_SrcRect);
	}
	glPopMatrix();

	utils::SetColor(Color4f(0.5, 0.5, 1, 1));
	utils::DrawLine(m_HitInfo.intersectPoint, Point2f(m_HitInfo.intersectPoint.x + m_HitInfo.normal.x * 30, m_HitInfo.intersectPoint.y + m_HitInfo.normal.y * 30), 2);
	
	
	// I LOVE DEBUGGING
	const Point2f topLeft{ m_Center.x - (m_Width / 2), m_Center.y + (m_Height / 2) };
	const Point2f topRight{ m_Center.x + (m_Width / 2), m_Center.y + (m_Height / 2) };
	const Point2f bottomLeft{ m_Center.x - (m_Width / 2), m_Center.y - (m_Height / 2) + 1 };
	const Point2f bottomRight{ m_Center.x + (m_Width / 2), m_Center.y - (m_Height / 2) + 1 };
	const Point2f halfLeft{ m_Center.x - (m_Width / 2), m_Center.y };
	const Point2f halfRight{ m_Center.x + (m_Width / 2), m_Center.y };

	utils::SetColor(Color4f(1, 1, 0, 1));
	utils::DrawLine(bottomLeft, topLeft, 4);
	utils::SetColor(Color4f(1, 0, 1, 1));
	utils::DrawLine(bottomRight, topRight, 4);
	utils::SetColor(Color4f(1, 0, 0, 1));
	utils::DrawLine(halfLeft, halfRight, 4);
}

void Entity::SetPosition(const Point2f& newCenterPos)
{
	m_Center = newCenterPos;
}

void Entity::SetPosition(float centerX, float centerY)
{
	SetPosition(Point2f(centerX, centerY));
}

void Entity::IsEliminated(bool isEliminated)
{
	m_IsEliminated = isEliminated;
}

Point2f Entity::GetPosition() const
{
	return m_Center;
}

Rectf Entity::GetDstRect() const
{
	const Rectf dstRect
	{
		m_Center.x - m_Width  / 2,
		m_Center.y - m_Height / 2,
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

void Entity::FloorCollision(const std::vector<std::vector<Point2f>>& world)
{
	Collision::FloorCollision(this, world);
}

void Entity::WallCollision(const std::vector<std::vector<Point2f>>& world)
{
	Collision::WallCollision(this, world);
}

void Entity::UpdateSourceRect()
{
	m_SrcRect.left		= m_SrcRectStart.x + m_CurrentFrame * m_Width;
	m_SrcRect.bottom	= m_SrcRectStart.y - m_CurrentFrameRow  * m_Height;
	m_SrcRect.width		= m_Width;
	m_SrcRect.height	= m_Height;
}

void Entity::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += m_GRAVITY * elapsedSec;
	m_Center.y += m_Velocity.y * elapsedSec;
}

