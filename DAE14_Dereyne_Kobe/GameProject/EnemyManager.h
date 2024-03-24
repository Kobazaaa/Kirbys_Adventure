#pragma once
#include "WaddleDee.h"
#include "BrontoBurt.h"

class Kirby;

class EnemyManager
{
public:
	// Constructor & Destructor
	EnemyManager();
	~EnemyManager();

	// Behavioral
	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	void Add(const std::string& filePath, Point2f center);
	void Add(Enemy* enemyPtr);
	void Pop();
	void Eliminate(int index);

	bool KirbyCollision(const Rectf& kirbyHitbox);
	bool KirbyInhaleCollision(const Rectf& inhaleRect, float elapsedSec, const Point2f& kirbyPos);

private:
	// Variables
	std::vector<Enemy*> m_vEnemies;
};

