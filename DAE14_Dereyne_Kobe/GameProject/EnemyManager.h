#pragma once
#include "WaddleDee.h"
#include "BrontoBurt.h"
#include "WaddleDoo.h"
#include "Sparky.h"
#include "HotHead.h"
#include "PoppyBrosJr.h"

class Kirby;
class Camera;

class EnemyManager final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit EnemyManager(Camera* camera);
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager(const EnemyManager&& other) = delete;
	EnemyManager& operator=(const EnemyManager& rhs) = delete;
	EnemyManager& operator=(const EnemyManager&& rhs) = delete;
	virtual ~EnemyManager() noexcept;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Draw() const;
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& world, const Point2f& kirbyPos);

	void DoUnderwaterChecks(const std::vector<std::vector<Point2f>>& world);

	void Add(const std::string& filePath, Point2f center, int score);
	void Add(Enemy* enemyPtr);
	void Pop();
	void Eliminate(Enemy* enemyPtr);

	bool KirbyInhaleCollision(Kirby* pKirby, float elapsedSec);

	std::vector<Enemy*>& GetAllEnemies();

private:
	// Function
	bool IsEnemyInhaleRect(Kirby* pKirby) const;
	void ApplyPlaySpace();

	// Variables
	Camera* m_pCamera;
	std::vector<Enemy*> m_vEnemies;
};

