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

	// Constructor & Destructor
	Entity(const std::string& spriteFilePath, float width, float height, const Point2f& center);
	virtual ~Entity();

	// Behavioral
	void Draw(bool flipSprite) const;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	// Mutators
	void SetPosition(const Point2f& newCenterPos);
	void SetPosition(float centerX, float centerY);
	void IsEliminated(bool isEliminated);

	// Accessors
	Point2f	GetPosition()	 const;
	Rectf GetDstRect()		 const;
	float GetWidth()		 const;
	float GetHeight()		 const;
	Rectf GetHitBox()		 const;

	friend bool Collision::WallCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);
	friend bool Collision::FloorCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);
	friend bool Collision::EntityCollision	(Entity* entity, const std::vector<std::vector<Point2f>>& world);


protected:
	// Behavioral Functions
	
	void FloorCollision(const std::vector<std::vector<Point2f>>& world);
	void WallCollision(const std::vector<std::vector<Point2f>>& world);

	// Protected Data Members
	Point2f			m_Center;
	Rectf			m_HitBox;
	Vector2f		m_Velocity;
	Direction		m_Direction;
	utils::HitInfo	m_HitInfo;

	int m_CurrentFrame;
	int m_CurrentFrameRow;
	float m_Width;
	float m_Height;
	float m_AccumSec;
	bool m_IsInvincible;
	bool m_IsEliminated;
	bool m_WorldCollision;

	Point2f  m_SrcRectStart;

	float m_GRAVITY{ -300.f };

private:
	void UpdateSourceRect();
	void ApplyGravity(float elapsedSec);

	// Private Data Members
	Texture* m_pSpriteSheet;
	Rectf	 m_SrcRect;

};

