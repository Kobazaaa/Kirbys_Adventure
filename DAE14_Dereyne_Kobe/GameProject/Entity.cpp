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
	, m_WorldCollision	{ false }
	, m_IsEliminated	{ false }
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
	WorldCollision(world);
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

	utils::SetColor(Color4f(1, 0, 0, 1));
	utils::DrawLine(m_HitInfo.intersectPoint, Point2f(m_HitInfo.intersectPoint.x + m_HitInfo.normal.x * 30, m_HitInfo.intersectPoint.y + m_HitInfo.normal.y * 30), 3);
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

void Entity::WorldCollision(const std::vector<std::vector<Point2f>>& world)
{
	const Point2f topLeft		{ m_Center.x - (m_Width / 2), m_Center.y + (m_Height / 2) };
	const Point2f topRight		{ m_Center.x + (m_Width / 2), m_Center.y + (m_Height / 2) };
	const Point2f bottomLeft	{ m_Center.x - (m_Width / 2), m_Center.y - (m_Height / 2) };
	const Point2f bottomRight	{ m_Center.x + (m_Width / 2), m_Center.y - (m_Height / 2) };

	for (size_t vectorIdx{ 0 }; vectorIdx < world.size(); ++vectorIdx)
	{
		//if (utils::Raycast(world[vectorIdx], topLeft, topRight, m_HitInfo) or utils::Raycast(world[vectorIdx], bottomLeft, bottomRight, m_HitInfo))
		//{
		//	m_Velocity.y = 0;
		//	m_Center = (Point2f(m_Center.x, m_HitInfo.intersectPoint.y + (m_Height / 2)));
		//	m_WorldCollision = true;
		//	//m_Velocity.x = 0;
		//	//m_Center = (Point2f(m_HitInfo.intersectPoint.x - static_cast<int>(m_Direction) * (m_Width / 2), m_Center.y));
		//	//m_WorldCollision = true;
		//}
		//else m_WorldCollision = false;

		if (utils::Raycast(world[vectorIdx], bottomLeft, topLeft, m_HitInfo) or utils::Raycast(world[vectorIdx], topRight, bottomRight, m_HitInfo))
		{
			m_Velocity.y = 0;
			m_Center = (Point2f(m_Center.x, m_HitInfo.intersectPoint.y + (m_Height / 2)));
			m_WorldCollision = true;
		}
		else
		{
			m_WorldCollision = false;
		}
		

		

	}
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

