#include "pch.h"
#include "Level.h"

Level::Level(const std::string& filePath, int nrSubLevels)
	: m_pTexture		{ new Texture(filePath)}
	, m_NR_SUB_LEVELS	{ nrSubLevels }
	, m_CurrentSubLevel	{ 0 }
{
	m_Width = m_pTexture->GetWidth();
	m_Height = m_pTexture->GetHeight() / m_NR_SUB_LEVELS;
	m_Position = Point2f(0, 232.f - m_Height);
}

Level::~Level()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Level::Draw() const
{
	const Rectf srcRect
	{
		0, -m_CurrentSubLevel * m_Height, m_Width, m_Height
	};
	m_pTexture->Draw(m_Position, srcRect);

}

float Level::GetWidth() const
{
	return m_Width;
}

float Level::GetHeight() const
{
	return m_Height;
}

Point2f Level::GetPosition() const
{
	return m_Position;
}

int Level::GetSubLevel() const
{
	return m_CurrentSubLevel;
}

void Level::IncrSubLevel()
{
	++m_CurrentSubLevel;
}

void Level::DecrSubLevel()
{
	--m_CurrentSubLevel;
}

void Level::SetSubLevel(int subLevel)
{
	m_CurrentSubLevel = subLevel;
}
