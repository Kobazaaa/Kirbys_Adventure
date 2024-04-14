#pragma once
#include "Texture.h"
#include <vector>

struct Door
{
	Rectf doorRect;
	Point2f outcomePos;
	bool isFinalDoor;
};

class Level final
{
public:

	// Constructor & Destructor
	explicit Level(const std::string& filePath, int nrSubLevels);
	Level(const Level& other) = delete;
	~Level();
	Level& operator=(const Level& rhs) = delete;

	// Behavioral
	void	Draw()			const;

	// Accessors
	float	GetWidth()			 const;
	float	GetHeight()			 const;
	float	GetSubLevelHeight()	 const;

	int		GetNrSubLevels()	 const;
	int		GetCurrentSubLevel() const;

	std::vector<Door>& GetDoors();

	// Mutators
	void IncreaseSubLevel();
	void DecreaseSubLevel();
	void SetSubLevel(int subLevel);

private:
	// Variables
	Point2f m_Position;
	Texture* m_pTexture;
	std::vector<Door> m_vDoors;


	float m_Width;
	float m_Height;
	int m_NrSubLevels;
	int m_CurrentSubLevel;
};

