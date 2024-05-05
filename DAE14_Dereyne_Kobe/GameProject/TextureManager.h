#pragma once
#include "Texture.h"
#include <vector>

class TextureManager final
{
public:
	struct TextureObj
	{
		explicit TextureObj(const std::string& name, const std::string& filepath);
		TextureObj(const TextureObj& other);
		TextureObj(TextureObj&& other) noexcept;
		TextureObj& operator=(const TextureObj& rhs) = delete;
		TextureObj& operator=(TextureObj&& rhs) = delete;
		~TextureObj();

	public:
		std::string m_Name;
		Texture* m_pTexture;

	private:
		std::string m_FilePath;
	};

	static bool LoadTexture(const std::string& name, const std::string& filePath);
	static void DeleteTextures();
	static Texture* GetTexture(int index);
	static Texture* GetTexture(const std::string& name);

private:
	static bool IsTextureLoaded(const std::string& name);
	static std::vector<TextureObj*> m_vTextures;
};

