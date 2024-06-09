#pragma once
#include "utils.h"
#include "SoundManager.h"
#include "AnimationManager.h"
#include "WorldUtils.h"
#include "Vector2f.h"

class Projectile
{
public:
	enum class Element
	{
		None, Fire, Electricity
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Projectile(const std::string& textureName, const Vector2f velocity, float travelTime, bool isFriendly = false, float customHitBoxSize = 4.f);
	Projectile(const Projectile& other) = delete;
	Projectile(Projectile&& other) = delete;
	Projectile& operator=(const Projectile& rhs) = delete;
	Projectile& operator=(Projectile&& rhs) = delete;
	virtual ~Projectile() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, int index = 0) = 0;
	virtual void Draw() const;

	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Rectf GetHitBox() const;
	const utils::HitInfo& GetHitInfo() const;

	bool IsActivated() const;
	bool IsFriendly() const;
	bool DoesEntityCollision() const;
	bool DoesParticleOnImpact() const;
	bool DoesDeactivateOnImpact() const;
	Point2f GetPosition() const;
	Direction GetDirection() const;
	float GetWidth() const;
	float GetHeight() const;
	Element GetElement();

	static std::vector<Projectile*>& GetAllProjectiles();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Activate(const Point2f& position, Direction direction);
	void Hide();
	void Reveal();
	void Deactivate();
	void SetVelocity(const Vector2f& velocity);
	void SetPosition(const Point2f& position);
	void SetStartPosition(const Point2f& position);

protected:
	// Protected Variables
	float			m_TravelTime;
	bool			m_IsActive;
	bool			m_IsFriendly;
	bool			m_DoesEntityCollision;
	bool			m_Hidden;
	bool			m_ParticlesOnImpact;
	bool			m_DeactivateOnImpact;

	Point2f			m_Position;
	Point2f			m_StartPosition;
	Vector2f		m_Velocity;
	Direction		m_Direction;
	float			m_AccumSec;	

	AnimationManager* m_pAnimationManager;
	std::string m_CurrentAnimation;

	Element m_Element;
private:
	// Private Variables
	utils::HitInfo	m_HitInfo;

	float			m_Width;
	float			m_Height;
	float			m_CustomHitBoxSize;

	static std::vector<Projectile*> m_vAllProjectiles;
};

