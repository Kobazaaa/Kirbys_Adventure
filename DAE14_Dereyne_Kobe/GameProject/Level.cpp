#include "pch.h"
#include "TextureManager.h"
#include "Level.h"

Level::Level(const std::string& name, int nrSubLevels)
	: m_Position		{ Point2f(0, 0) }
	, m_NrSubLevels		{ nrSubLevels }
	, m_CurrentSubLevel	{ 0 }
	, m_pTexture		{ TextureManager::GetTexture(name) }
{
	m_Width		= m_pTexture->GetWidth();
	m_Height	= m_pTexture->GetHeight();

	Door door1
	{
		Rectf(968, 57, 16, 24),
		Point2f(28, 228),
		false
	};
	Door door3
	{
		Rectf(968, 209, 16, 24),
		Point2f(32, 386),
		false
	};
	Door door2
	{
		Rectf(24, 377, 16, 24),
		Point2f(976, 218),
		false
	};
	m_vDoors.push_back(door1);
	m_vDoors.push_back(door2);
	m_vDoors.push_back(door3);


}

void Level::Draw() const
{
	const Rectf srcRect
	{
		0, -m_Height, m_Width, m_Height
	};
	m_pTexture->Draw(m_Position, srcRect);

}

#pragma region Accessors
float Level::GetWidth() const
{
	return m_Width;
}

float Level::GetHeight() const
{
	return m_Height;
}

std::vector<Door>& Level::GetDoors()
{
	return m_vDoors;
}

int Level::GetNrSubLevels() const
{
	return m_NrSubLevels;
}

float Level::GetSubLevelHeight() const
{
	return m_Height / m_NrSubLevels;
}

int Level::GetCurrentSubLevel() const
{
	return m_CurrentSubLevel;
}
#pragma endregion

#pragma region Mutators
void Level::IncreaseSubLevel()
{
	++m_CurrentSubLevel;
}

void Level::DecreaseSubLevel()
{
	--m_CurrentSubLevel;
}

void Level::SetSubLevel(int subLevel)
{
	m_CurrentSubLevel = subLevel;
}
#pragma endregion