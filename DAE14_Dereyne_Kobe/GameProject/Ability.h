#pragma once
#include <vector>
#include "Entity.h"
#include "Texture.h"
#include "Projectile.h"

class Ability
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Ability(bool canBeCancelled);
	Ability(const Ability& other) = delete;
	Ability(Ability&& other) = delete;
	Ability& operator=(const Ability& rhs) = delete;
	Ability& operator=(Ability&& rhs) = delete;
	virtual ~Ability() noexcept;
	

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner) = 0;
	virtual void Draw() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual void Activate(const Point2f& position, Direction direction) = 0;
	virtual void Activate();
	void Deactivate();


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const std::vector<Projectile*>& GetProjectiles() const;
	bool IsActive() const;
	bool CanBeCancelled() const;

protected:
	// Protected VariablesS
	std::vector<Projectile*> m_vProjectiles;

	bool m_IsActive;
	float m_AccumSec;

private:
	// Private Variables
	bool m_CanBeCancelled;
};

