#include "pch.h"
#include <iostream>
#include "Entity.h"

const float Entity::GRAVITY {-300.f};
const float Entity::SCALE	{ 2.f };

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
	const float scaledWidth	{ m_Width  * SCALE };
	const float scaledHeight{ m_Height * SCALE };
	const Rectf dstRect
	{
		m_Center.x - scaledWidth  / 2,
		m_Center.y - scaledHeight / 2,
		scaledWidth,
		scaledHeight,
	};
	
	glPushMatrix();
	{
		if (flipSprite)
		{
			glTranslatef(m_Center.x, 0, 0);
			glRotatef(-180, 0, 1, 0);
			glTranslatef(-m_Center.x, 0, 0);
		}
		m_pSpriteSheet->Draw(dstRect, m_SrcRect);
	}
	glPopMatrix();
}

void Entity::SetPosition(const Point2f& newCenterPos)
{
	m_Center = newCenterPos;
}

void Entity::SetPosition(float centerX, float centerY)
{
	SetPosition(Point2f(centerX, centerY));
}

void Entity::WorldCollision(const std::vector<std::vector<Point2f>>& world)
{
	Point2f middleBottom{ m_Center.x, m_Center.y - (m_Width  / 2) * SCALE -1 };
	Point2f middleTop	{ m_Center.x, m_Center.y + (m_Height / 2) * SCALE };

	for (size_t vectorIdx{ 0 }; vectorIdx < world.size(); ++vectorIdx)
	{
		if (utils::Raycast(world[vectorIdx], middleTop, middleBottom, m_HitInfo))
		{
			m_Velocity.y = 0;
			m_Center = (Point2f(m_Center.x, m_HitInfo.intersectPoint.y + (m_Height / 2) * SCALE));
		}
	}
}

void Entity::UpdateSourceRect()
{
	m_SrcRect.left		= m_CurrentFrame * m_Width;
	m_SrcRect.bottom	= -m_CurrentFrameRow  * m_Height;
	m_SrcRect.width		= m_Width;
	m_SrcRect.height	= m_Height;
}

void Entity::ApplyGravity(float elapsedSec)
{
	m_Velocity.y += GRAVITY * elapsedSec;
	m_Center.y += m_Velocity.y * elapsedSec;
}

