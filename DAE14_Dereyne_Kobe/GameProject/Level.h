#pragma once
#include "Texture.h"
class Level
{
public:
	Level(const std::string& filePath);
	~Level();

	void Draw() const;

	float GetWidth() const;
	float GetHeight() const;
private:
	Point2f m_Position;
	Texture* m_pTexture;
	float m_Width;
	float m_Height;

};

