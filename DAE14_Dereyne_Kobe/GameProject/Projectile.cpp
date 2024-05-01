#include "pch.h"
#include "Collision.h"
#include "TextureManager.h"
#include "Projectile.h"

Projectile::Projectile(const std::string& textureName, const Vector2f velocity, float travelTime, bool isFriendly, float customHitBoxSize)
	: m_pTexture	{ TextureManager::GetTexture(textureName) }
	, m_Width		{ 16 }
	, m_Height		{ 16 }
	, m_Velocity	{ velocity}
	, m_Direction	{ Direction::Right }
	, m_Position	{ Point2f(0,0) }
	, m_IsActive	{ false }
	, m_IsFriendly	{ isFriendly }
	, m_TravelTime	{ travelTime }
	, m_CurrentFrame	{ 0 }
	, m_CurrentFrameRow	{ 0 }
	, m_AccumSec		{ 0.f }
	, m_Hidden			{false}
	, m_CustomHitBoxSize{customHitBoxSize}
{
}

void Projectile::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index)
{
	if (m_IsActive)
	{
		m_AccumSec += elapsedSec;

		// MOVE
		m_Position.x += static_cast<int>(m_Direction) * m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		// RESET
		if (m_AccumSec >= m_TravelTime)
		{
			if (m_AccumSec >= m_TravelTime + 0.3f)
			{
				m_IsActive = false;
				m_AccumSec = 0;
			}
		}

		// COLLSION
		if (Collision::WallCollision(this, world))
		{
			m_IsActive = false;
			m_AccumSec = 0;
		}
	}
}

void Projectile::Draw() const
{
	if (m_IsActive and !m_Hidden)
	{
		bool flipSprite{ m_Direction == Direction::Left ? true : false };

		glPushMatrix();
		{
			if (flipSprite)
			{
				glTranslatef(m_Position.x, 0, 0);
				glRotatef(-180, 0, 1, 0);
				glTranslatef(-m_Position.x, 0, 0);
			}
			m_pTexture->Draw(GetDstRect(), GetSrcRect());
		}
		glPopMatrix();

		utils::SetColor(Color4f(0, 1, 1, 1));
		utils::DrawRect(GetHitBox());
		//utils::DrawRect(GetDstRect());

	}
}

Rectf Projectile::GetHitBox() const
{
	Rectf hitBox{};
	hitBox.width	= m_CustomHitBoxSize;
	hitBox.height	= m_CustomHitBoxSize;
	hitBox.left		= m_Position.x - hitBox.width /  2;
	hitBox.bottom	= m_Position.y - hitBox.height / 2;

	return hitBox;
}

const utils::HitInfo& Projectile::GetHitInfo() const
{
	return m_HitInfo;
}

Rectf Projectile::GetDstRect() const
{
	const Rectf dstRect
	{
		m_Position.x - m_Width / 2,
		m_Position.y - m_Height / 2,
		m_Width,
		m_Height,
	};
	return dstRect;
}

Rectf Projectile::GetSrcRect() const
{
	Rectf srcRect
	{
		m_CurrentFrame * m_Width,
		m_CurrentFrameRow * m_Height,
		m_Width, m_Height
	};

	return srcRect;
}

bool Projectile::IsActivated()
{
	return m_IsActive;
}

bool Projectile::IsFriendly()
{
	return m_IsFriendly;
}

Point2f Projectile::GetPosition()
{
	return m_Position;
}

Direction Projectile::GetDirection()
{
	return m_Direction;
}

float Projectile::GetWidth()
{
	return m_Width;
}

float Projectile::GetHeight()
{
	return m_Height;
}

void Projectile::Activate(const Point2f& position, Direction direction)
{
	m_Position = position;
	m_StartPosition = position;
	m_Direction = direction;
	m_IsActive = true;	
	m_AccumSec = 0;
}

void Projectile::Hide()
{
	m_Hidden = true;
}

void Projectile::Reveal()
{
	m_Hidden = false;
}

void Projectile::Deactivate()
{
	m_IsActive = false;
}

void Projectile::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}

void Projectile::SetPosition(const Point2f& position)
{
	m_Position = position;
}

void Projectile::SetStartPosition(const Point2f& position)
{
	m_StartPosition = position;
}
