#pragma once
#include "WaddleDee.h"
#include "BrontoBurt.h"
#include "WaddleDoo.h"
#include "Sparky.h"
#include "HotHead.h"

class Kirby;
class Camera;

class EnemyManager final
{
public:
	// Constructor & Destructor
	explicit EnemyManager(Camera* camera);
	EnemyManager(const EnemyManager& other) = delete;
	~EnemyManager();
	EnemyManager& operator=(const EnemyManager& rhs) = delete;

	// Behavioral
	void Draw(bool debugMode = false) const;
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world);

	void Add(const std::string& filePath, Point2f center);
	void Add(Enemy* enemyPtr);
	void Pop();
	void Eliminate(Enemy* enemyPtr);

	bool EnemyHitKirbyDetection(Kirby* pKirby);
	bool KirbyInhaleCollision(Kirby* pKirby, float elapsedSec);
	bool EnemyKirbyProjectileCollision(Kirby* pKirby);

private:
	// Function
	bool IsEnemyInhaleRect(Kirby* pKirby) const;
	void ApplyPlaySpace();

	// Variables
	Camera* m_pCamera;
	std::vector<Enemy*> m_vEnemies;
};

