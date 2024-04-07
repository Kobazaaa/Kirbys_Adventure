#pragma once
#include "utils.h"
#include "Texture.h"
#include "Collision.h"

class Entity
{
public:
	enum class Direction
	{
		Left = -1,
		Right = 1,
	};

	// Constructor, Destructor and Operator Overloading
	explicit Entity(const std::string& spriteFilePath, float width, float height, const Point2f& center);
	Entity(const Entity& other) = delete;
	virtual ~Entity();
	Entity& operator=(const Entity& rhs) = delete;

	// Behavioral
	virtual void Draw(bool flipSprite) const;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	// Mutators
	virtual void SetPosition(const Point2f& newCenterPos)		final;
	virtual void SetPosition(float centerX, float centerY)		final;
	virtual void SetVelocity(const Vector2f& velocity)			final;
	virtual void IsEliminated(bool isEliminated)				final;
	virtual void InverseDirection()								final;

	// Accessors
	virtual Point2f	GetPosition()			 const final;
	virtual Vector2f GetVelocity()			 const final;
	virtual float GetWidth()				 const final;
	virtual float GetHeight()				 const final;
	virtual Rectf GetHitBox()				 const final;
	virtual utils::HitInfo& GetHitInfo()		   final;
	virtual Entity::Direction GetDirection() const final;

protected:
	// Behavioral Functions
	
	// Protected Data Members
	Point2f			m_Position;
	Vector2f		m_Velocity;
	Direction		m_Direction;
	utils::HitInfo	m_HitInfo;

	int		m_CurrentFrame;
	int		m_CurrentFrameRow;
	float	m_AccumSec;

	float	m_Width;
	float	m_Height;

	bool	m_IsInvincible;
	bool	m_IsEliminated;

	Point2f  m_SrcRectStart;

	static const float m_GRAVITY;

private:
	void UpdateSourceRect();
	void ApplyGravity(float elapsedSec);

	Rectf GetDstRect()				 const;
	Point2f GetDstCenter()			 const;
	// Private Data Members
	Texture* m_pSpriteSheet;
	Rectf	 m_SrcRect;
};

