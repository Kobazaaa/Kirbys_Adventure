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
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit Level(const std::string& filePath, int nrSubLevels);
	virtual ~Level() noexcept = default;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void	Draw() const;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	float	GetWidth()			 const;
	float	GetHeight()			 const;
	float	GetSubLevelHeight()	 const;

	int		GetNrSubLevels()	 const;
	int		GetCurrentSubLevel() const;

	std::vector<Door>& GetDoors();


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


	float m_Width;
	float m_Height;
	int m_NrSubLevels;
	int m_CurrentSubLevel;
};

