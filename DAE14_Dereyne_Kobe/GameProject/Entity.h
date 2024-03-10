#pragma once
#include "utils.h"
#include "Texture.h"

class Entity
{
public:
	enum class Direction
	{
		Left = -1,
		Right = 1,
	};

	// constructors & destructor
	Entity(const std::string& spriteFilePath, float width, float height, const Point2f& center);
	~Entity();

	// Behavioral Functions
	void Draw(bool flipSprite) const;

	// Mutators
	void SetPosition(const Point2f& newCenterPos);
	void SetPosition(float centerX, float centerY);

protected:
	// Behavioral Functions
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);
	
	void WorldCollision(const std::vector<std::vector<Point2f>>& world);

	// Protected Data Members
	Point2f			m_Center;
	Vector2f		m_Velocity;
	Direction		m_Direction;
	utils::HitInfo	m_HitInfo;

	int m_CurrentFrame;
	int m_CurrentFrameRow;
	float m_Width;
	float m_Height;
	float m_AccumSec;

	Point2f  m_SrcRectStart;
	// Static Data Members
	static const float GRAVITY;
	static const float SCALE;
private:
	void UpdateSourceRect();
	void ApplyGravity(float elapsedSec);

	// Private Data Members
	Texture* m_pSpriteSheet;
	Rectf	 m_SrcRect;
};

