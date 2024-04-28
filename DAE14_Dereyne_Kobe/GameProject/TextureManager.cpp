#include "pch.h"
#include <iostream>
#include "TextureManager.h"

std::vector<TextureManager::TextureObj> TextureManager::m_vTextures;

TextureManager::TextureManager()
{
	// I need this constructor so in Game.h/Game.cpp I can create a TextureManager object, even though I don't need it.
	// I need the object so that I can also delete it so the destructor gets called to delete my static vector which has heap objects.
}
TextureManager::~TextureManager()
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		delete m_vTextures[index].m_pTexture;
		m_vTextures[index].m_pTexture = nullptr;
	}
}

bool TextureManager::LoadTexture(const std::string& name, const std::string& filePath)
{
	if (!IsTextureLoaded(name))
	{
		m_vTextures.push_back(TextureObj(name, filePath));

		if (!m_vTextures.at(m_vTextures.size() - 1).m_pTexture->IsCreationOk())
		{
			std::cout << "TEXTURE_MANAGER: Could not load texture with name \"" << name << "\" and filepath \"" << filePath << "\" correctly\n";
			return false;
		}
	}
	
	std::cout << m_vTextures.size() << std::endl;

	return true;
}

Texture* TextureManager::GetTexture(int index)
{
	return m_vTextures.at(index).m_pTexture;
}
Texture* TextureManager::GetTexture(const std::string& name)
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		if (m_vTextures[index].m_Name == name)
		{
			return m_vTextures[index].m_pTexture;
		}
	}
	return nullptr;
}

bool TextureManager::IsTextureLoaded(const std::string& name)
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		if (m_vTextures[index].m_Name == name)
		{
			std::cout << "TEXTURE_MANAGER: Another texture has already been created with the name \"" << name << "\", please try again with another name\n";
			return true;
		}
	}
	return false;
}

TextureManager::TextureObj::TextureObj(const std::string& name, const std::string& filepath)
	: m_Name{ name }
	, m_pTexture{ new Texture(filepath) }
{
}