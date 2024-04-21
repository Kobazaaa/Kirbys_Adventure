#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	// Constructor & Destructor
	explicit Enemy(const std::string& filePathName, const Point2f& center, Ability::Type abilityType, bool doesWorldCollsion = true);
	virtual ~Enemy() override = default;

	// Behavioral
	virtual void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world) override;
	virtual void Draw() const;

	// Accessors
	bool IsActivated() const;
	bool IsEliminated() const;
	Rectf GetSpawnRect() const;
	Point2f GetSpawnPoint() const;

	// Mutators
	void IsActivated(bool isActivated);
	void IsEliminated(bool isEliminated);
	void Reset();

protected:
	bool m_DoesWorldCollision;

	const float m_WALK_SPEED{30.f};

	const float m_WALK_FRAME_DELAY{0.2f};
private:
	// Variables
	bool m_IsEliminated;
	bool m_IsActivated;

	Point2f m_SpawnPoint;
};

