#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	// Constructors and Destructor
	Enemy(const std::string& filePathName, const Point2f& center);
	~Enemy();

	// Behavioral Functions
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);
	void Draw() const;

	void IsActivated(bool isActivated);

private:
	bool m_IsActivated;
};

