#pragma once
#include "Texture.h"

class Level final
{
public:
	struct Door
	{
		bool m_Advance;
		const Rectf m_Door;
	};

	// Constructor & Destructor
	Level(const std::string& filePath, int nrSubLevels);
	~Level();

	// Behavioral
	void	Draw()			const;

	// Accessors
	float	GetWidth()		const;
	float	GetHeight()		const;
	Point2f GetPosition()	const;
	int		GetSubLevel()	const;

	// Mutators
	void IncrSubLevel();
	void DecrSubLevel();
	void SetSubLevel(int subLevel);

private:
	// Variables
	Point2f m_Position;
	Texture* m_pTexture;

	const int m_NR_SUB_LEVELS;

	float m_Width;
	float m_Height;
	int m_CurrentSubLevel;

};

