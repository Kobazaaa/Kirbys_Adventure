#pragma once
#include <vector>
#include "Entity.h"
#include "Texture.h"
#include "Projectile.h"

class Ability
{
public:
	explicit Ability(bool canBeCancelled);
	Ability(const Ability& other) = delete;
	Ability(Ability&& other) = delete;
	Ability& operator=(const Ability& rhs) = delete;
	Ability& operator=(Ability&& rhs) = delete;
	virtual ~Ability();
	
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, Entity* owner) = 0;
	virtual void Draw() const;

	virtual void Activate(const Point2f& position, Direction direction) = 0;
	virtual void Activate();
	void Deactivate();

	const std::vector<Projectile*>& GetProjectiles() const;
	bool IsActive() const;
	bool CanBeCancelled() const;

protected:
	bool m_IsActive;
	std::vector<Projectile*> m_vProjectiles;
	float m_AccumSec;
private:
	bool m_CanBeCancelled;
};

