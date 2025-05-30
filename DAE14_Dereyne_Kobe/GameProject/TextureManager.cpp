#include "pch.h"
#include "utils.h"
#include <iostream>
#include "TextureManager.h"

std::vector<TextureManager::TextureObj*> TextureManager::m_vTextures;

bool TextureManager::LoadTexture(const std::string& name, const std::string& filePath)
{
	if (!IsTextureLoaded(name))
	{
		m_vTextures.push_back(std::move(new TextureObj{ name, filePath }));
		
		if (!m_vTextures.at(m_vTextures.size() - 1)->m_pTexture->IsCreationOk())
		{
			std::cout << "TEXTURE_MANAGER: Could not load texture with name \"" << name << "\" and filepath \"" << filePath << "\" correctly\n";
			return false;
		}
	}
	
	if (utils::DEBUG_MODE) std::cout << m_vTextures.size() << "\t Texture" << std::endl;

	return true;
}

void TextureManager::DeleteTextures() noexcept
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		delete m_vTextures[index];
		m_vTextures[index] = nullptr;
	}
	m_vTextures.clear();
}

Texture* TextureManager::GetTexture(int index)
{
	return m_vTextures.at(index)->m_pTexture;
}
Texture* TextureManager::GetTexture(const std::string& name)
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		if (m_vTextures[index]->m_Name == name)
		{
			return m_vTextures[index]->m_pTexture;
		}
	}
	return nullptr;
}

bool TextureManager::IsTextureLoaded(const std::string& name)
{
	for (int index{}; index < m_vTextures.size(); ++index)
	{
		if (m_vTextures[index]->m_Name == name)
		{
			std::cout << "TEXTURE_MANAGER: Another texture has already been created with the name \"" << name << "\", please try again with another name\n";
			return true;
		}
	}
	return false;
}

#pragma region TextureObject
TextureManager::TextureObj::TextureObj(const std::string& name, const std::string& filepath)
	: m_Name		{ name }
	, m_pTexture	{ new Texture(filepath) }
	, m_FilePath	{ filepath }
{
}

TextureManager::TextureObj::TextureObj(const TextureObj& other)
{
	delete this->m_pTexture;

	this->m_Name		=		other.m_Name;
	this->m_pTexture	=		new Texture(other.m_FilePath);
	this->m_FilePath	=		other.m_FilePath;
}

TextureManager::TextureObj::TextureObj(TextureObj&& other) noexcept
{
	delete this->m_pTexture;

	this->m_Name		=		std::move(other.m_Name);
	this->m_FilePath	=		std::move(other.m_FilePath);
	this->m_pTexture	=		std::move(other.m_pTexture);

	other.m_pTexture	=		nullptr;
}

TextureManager::TextureObj::~TextureObj()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}
#pragma endregion