#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	// Constructor & Destructor
	Enemy(const std::string& filePathName, const Point2f& center);
	virtual ~Enemy() override;

	// Behavioral
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);
	void Draw() const;

	// Accessors
	bool IsActivated() const;

	// Mutators
	void IsActivated(bool isActivated);

private:
	// Variables
	bool m_IsActivated;
};

