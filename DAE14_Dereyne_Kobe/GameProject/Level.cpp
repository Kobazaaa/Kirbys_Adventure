#include "pch.h"
#include "Kirby.h"
#include "SVGParser.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Level.h"

Level::Level(const std::string& name, int nrSubLevels, EnemyManager* enemies, std::vector<Door> doors)
	: m_Position		{ Point2f(0, 0) }
	, m_NrSubLevels		{ nrSubLevels }
	, m_CurrentSubLevel	{ 0 }
	, m_pTexture		{ TextureManager::GetTexture(name) }
	, m_pEnemyMngr		{ enemies }
	, m_vDoors			{ doors }
	, m_Name			{ name }
{
	m_Width		= m_pTexture->GetWidth();
	m_Height	= m_pTexture->GetHeight();
	
	SVGParser::GetVerticesFromSvgFile("Levels/" + name + ".svg", m_World);
}

Level::~Level() noexcept
{
	delete m_pEnemyMngr;
	m_pEnemyMngr = nullptr;
}

void Level::Update(float elapsedSec, Kirby* pKirby)
{
	if (m_pEnemyMngr) m_pEnemyMngr->Update(elapsedSec, m_World, pKirby->GetPosition());
	pKirby->ApplyPlaySpace(this);
}

void Level::Draw() const
{
	m_pTexture->Draw(m_Position);
	if (m_pEnemyMngr) m_pEnemyMngr->Draw();


	if (utils::DEBUG_MODE)
	{
		for (const Door& doorObj : m_vDoors)
		{
			utils::SetColor(Color4f(1, 0, 0, 1));
			utils::FillRect(doorObj.doorRect);
		}
	}
}

bool Level::IsOnDoor(Kirby* kirby, Door& door)
{
	for (const Door& doorObj : m_vDoors)
	{
		if (utils::IsPointInRect(kirby->GetPosition(), doorObj.doorRect))
		{
			kirby->EnterDoor();
			door = doorObj;
			return true;
		}
	}

	return false;
}

void Level::PlayMusic() const
{
	SoundManager::PlayStream(m_Name, -1);
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

const std::vector<std::vector<Point2f>>& Level::GetWorld() const
{
	return m_World;
}

EnemyManager* Level::GetEnemyMngr() const
{
	return m_pEnemyMngr;
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