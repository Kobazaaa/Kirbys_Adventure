#pragma once
#include "Texture.h"
#include "EnemyManager.h"
#include "PowerUp.h"
#include <vector>

class Kirby;

struct Door
{
	Rectf doorRect;
	Point2f outcomePos;
	bool isFinalDoor;
	int nextLevel;
};

class Level final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Level(const std::string& name, int nrSubLevels, EnemyManager* enemies, std::vector<Door> doors, std::vector<PowerUp*> powerUps, bool hasWater);
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level& operator=(Level&& rhs) = delete;
	virtual ~Level() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void	Update(float elapsedSec, Kirby* pKirby);
	void	Draw() const;
	bool	IsOnDoor(Kirby* kirby, Door& door);
	void	PlayMusic() const;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float	GetWidth()			 const;
	float	GetHeight()			 const;
	float	GetSubLevelHeight()	 const;

	int		GetNrSubLevels()	 const;
	int		GetCurrentSubLevel() const;

	std::vector<Door>& GetDoors();
	const std::vector<std::vector<Point2f>>& GetWorld() const;
	EnemyManager* GetEnemyMngr() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			MUTATORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void IncreaseSubLevel();
	void DecreaseSubLevel();
	void SetSubLevel(int subLevel);

private:
	// Private Variables
	Point2f m_Position;
	Texture* m_pTexture;
	
	std::vector<Door> m_vDoors;
	std::vector<std::vector<Point2f>> m_World;
	std::vector<std::vector<Point2f>> m_WaterBodies;

	std::vector<PowerUp*> m_vPowerUps;
	EnemyManager* m_pEnemyMngr;

	float m_Width;
	float m_Height;
	bool m_DoorEntered;
	bool m_HasWater;

	int m_NrSubLevels;
	int m_CurrentSubLevel;
	std::string m_Name;
};

