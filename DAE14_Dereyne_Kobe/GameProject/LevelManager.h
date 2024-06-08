#pragma once
#include <vector>
#include "EnemyManager.h"


class Level;
class Kirby;
class Camera;

class LevelManager final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit LevelManager(const std::string& filePath, Kirby* kirby, Camera* camera);
	LevelManager(const LevelManager& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	LevelManager& operator=(const LevelManager& rhs) = delete;
	LevelManager& operator=(LevelManager&& rhs) = delete;
	virtual ~LevelManager() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Update(float elapsedSec);
	void Draw() const;
	void Reset();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Level* GetCurrentLevel() const;


private:
	// Functions for file loading
	void LoadFromFile(const std::string& filePath);
	void AddLevel(const std::string& element);
	void AddEnemyManager(const std::string& element, EnemyManager* enemyManager);
	void AddDoors(const std::string& element, std::vector<Door>& doors);
	void AddPowerUps(const std::string& element, std::vector<PowerUp*>& powerUps);

	// Private Variables
	Level* m_CurrentLevel;
	std::vector<Level*> m_vLevels;

	Kirby* m_pKirby;
	bool m_DoorEntered;
	Camera* m_pCamera;
};

