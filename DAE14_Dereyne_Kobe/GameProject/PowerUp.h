#pragma once
#include "Vector2f.h"
#include "utils.h"
#include "TextureManager.h"
#include "AnimationManager.h"

class Kirby;

class PowerUp final
{
public:
	enum class Type
	{
		Tomato, Drink, Lollipop, LifeUp
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit PowerUp(const Point2f& position, PowerUp::Type powerUpType);
	PowerUp(const PowerUp& other) = delete;
	PowerUp(PowerUp&& other) = delete;
	PowerUp& operator=(const PowerUp& rhs) = delete;
	PowerUp& operator=(PowerUp&& rhs) = delete;
	~PowerUp() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Kirby* pKirby);
	void Draw() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Point2f GetPosition() const;
	Rectf GetHitBox() const;
	const utils::HitInfo& GetHitInfo() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Consume();
	void Respawn();
	void SetPosition(const Point2f& pos);
	void SetVelocity(const Vector2f& vel);

private:
	// Animate according to correct type
	void Animate(float elapsedSec, PowerUp::Type powerUpType);

	// Private Member Variables
	AnimationManager* m_pAnimationManager;
	utils::HitInfo m_HitInfo;
	Point2f m_Position;
	Vector2f m_Velocity;
	Type m_PowerUpType;

	bool m_IsConsumed;
};

