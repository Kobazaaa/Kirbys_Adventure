#pragma once
#include "Texture.h"
#include <vector>

class TextureManager final
{
public:
	struct TextureObj
	{
		explicit TextureObj(const std::string& name, const std::string& filepath);

		std::string m_Name;
		Texture* m_pTexture;
	};

	explicit TextureManager();
	TextureManager(const TextureManager& other) = delete;
	TextureManager(const TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager& rhs) = delete;
	TextureManager& operator=(TextureManager&& rhs) = delete;
	~TextureManager();

	static bool LoadTexture(const std::string& name, const std::string& filePath);
	
	static Texture* GetTexture(int index);
	static Texture* GetTexture(const std::string& name);

private:
	static bool IsTextureLoaded(const std::string& name);
	static std::vector<TextureObj> m_vTextures;
};

