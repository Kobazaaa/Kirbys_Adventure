#pragma once
#include "utils.h"
#include "Texture.h"
#include "AnimationManager.h"
#include "SoundManager.h"
#include "Direction.h"
#include "Collision.h"
#include "Ability.h"

class Entity
{
public:
	enum class AbilityType
	{
		None, Beam, Fire, Spark
	};

	// Constructor, Destructor and Operator Overloading
	explicit Entity(const std::string& textureName, float width, float height, const Point2f& center);
	Entity(const Entity& other) = delete;
	Entity(Entity&& other) = delete;
	Entity& operator=(const Entity& rhs) = delete;
	Entity& operator=(Entity&& rhs) = delete;
	virtual ~Entity();

	// Behavioral
	virtual void Draw(bool flipSprite) const;
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	// Mutators
	virtual void SetPosition(const Point2f& newCenterPos)		final;
	virtual void SetPosition(float centerX, float centerY)		final;
	virtual void SetVelocity(const Vector2f& velocity)			final;
	virtual void InverseDirection()								final;

	// Accessors
	virtual Point2f					GetPosition()		const final;
	virtual Vector2f				GetVelocity()		const final;
	virtual float					GetWidth()			const final;
	virtual float					GetHeight()			const final;
	virtual Rectf					GetHitBox()			const final;
	virtual utils::HitInfo&			GetHitInfo()		final;
	virtual Direction				GetDirection()		const final;
	virtual AbilityType				GetAbilityType()	const final;
	virtual Ability*				GetAbilityPtr()		const final;
protected:
	// Behavioral Functions
	
	// Protected Data Members
	AnimationManager* m_pAnimationManager;
	std::string m_CurrentAnimation;

	Point2f			m_Position;
	Vector2f		m_Velocity;
	Direction		m_Direction;
	utils::HitInfo	m_HitInfo;
	Ability*		m_pAbility;
	AbilityType		m_AbilityType;

	float	m_AccumSec;

	float	m_Width;
	float	m_Height;

	bool	m_IsInvincible;

	static const float m_GRAVITY;
	float m_GravityMultiplier;
private:
	void ApplyGravity(float elapsedSec);
};

