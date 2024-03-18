#include "pch.h"
#include "Level.h"

Level::Level(const std::string& filePath)
	: m_pTexture{ new Texture(filePath)}
{
	m_Width = m_pTexture->GetWidth();
	m_Height = m_pTexture->GetHeight();
	m_Position = Point2f(0, 232.f - m_Height);
}

Level::~Level()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Level::Draw() const
{
	m_pTexture->Draw(m_Position);
}

float Level::GetWidth() const
{
	return m_Width;
}

float Level::GetHeight() const
{
	return m_Height;
}
