#pragma once
#include "WaddleDee.h"
#include "BrontoBurt.h"

class Kirby;

class EnemyManager final
{
public:
	// Constructor & Destructor
	EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	~EnemyManager();
	EnemyManager& operator=(const EnemyManager& rhs) = delete;

	// Behavioral
	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	void Add(const std::string& filePath, Point2f center);
	void Add(Enemy* enemyPtr);
	void Pop();
	void Eliminate(int index);

	bool KirbyHitDetection(Kirby* pKirby);
	bool KirbyInhaleCollision(Kirby* pKirby, float elapsedSec);

private:
	// Variables
	std::vector<Enemy*> m_vEnemies;
};

