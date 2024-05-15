#pragma once
#include "utils.h"
#include "Direction.h"
#include "Texture.h"
#include "Vector2f.h"
#include "AnimationManager.h"

class Projectile
{
public:

	explicit Projectile(const std::string& textureName, const Vector2f velocity, float travelTime, bool isFriendly = false, float customHitBoxSize = 4.f);
	Projectile(const Projectile& other) = delete;
	Projectile(Projectile&& other) = delete;
	Projectile& operator=(const Projectile& rhs) = delete;
	Projectile& operator=(Projectile&& rhs) = delete;
	virtual ~Projectile();

	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) = 0;
	virtual void Draw() const;

	Rectf GetHitBox() const;
	const utils::HitInfo& GetHitInfo() const;

	bool IsActivated();
	bool IsFriendly();
	Point2f GetPosition();
	Direction GetDirection();
	float GetWidth();
	float GetHeight();

	void Activate(const Point2f& position, Direction direction);
	void Hide();
	void Reveal();
	void Deactivate();
	void SetVelocity(const Vector2f& velocity);
	void SetPosition(const Point2f& position);
	void SetStartPosition(const Point2f& position);

protected:
	float			m_TravelTime;
	bool			m_IsActive;
	bool			m_IsFriendly;
	bool			m_Hidden;

	Point2f			m_Position;
	Point2f			m_StartPosition;
	Vector2f		m_Velocity;
	float			m_AccumSec;	
	Direction		m_Direction;

	AnimationManager* m_pAnimationManager;
	std::string m_CurrentAnimation;
private:
	utils::HitInfo	m_HitInfo;

	float			m_Width;
	float			m_Height;
	float			m_CustomHitBoxSize;
};

