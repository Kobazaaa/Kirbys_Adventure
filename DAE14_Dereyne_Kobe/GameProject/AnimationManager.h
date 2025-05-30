#pragma once
#include "Animation.h"
#include "TextureManager.h"
#include "Texture.h"
#include <vector>

struct AnimObj
{
	explicit AnimObj(const std::string& name, Animation* animation);
	AnimObj(const AnimObj& other) = delete;
	AnimObj(AnimObj&& other) noexcept;
	AnimObj& operator=(const AnimObj& rhs) = delete;
	AnimObj& operator=(AnimObj&& rhs) noexcept;
	~AnimObj() noexcept;

	std::string m_Name;
	Animation* m_pAnimation;
};

class AnimationManager final
{
public:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~	CONSTRUCTOR & DESTRUCTOR	~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	explicit AnimationManager(Texture* spritesheet, bool oneAnimationAtATime = true);
	explicit AnimationManager(const std::string& textureName, bool oneAnimationAtATime = true);
	AnimationManager(const AnimationManager& other) = delete;
	AnimationManager(AnimationManager&& other) noexcept;
	AnimationManager& operator=(const AnimationManager& rhs) = delete;
	AnimationManager& operator=(AnimationManager&& rhs) noexcept;
	virtual ~AnimationManager() noexcept;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void Add(const std::string& name, Animation* animation);
	void LoadFromFile(const std::string& filePath);

	void Update(float elapsedSec, const std::string& animName);
	void Draw(const Point2f& position) const;
	void Deactivate(const std::string& animName);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			ACCESSORS			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsActive(const std::string& animName) const;
	bool IsDone(const std::string& animName) const;
	int GetCurrentFrame(const std::string& animName) const;
private:
	// Private Variables
	std::vector<AnimObj*> m_vAnimations;
	Texture* m_pSpriteSheet;
	bool m_OneAnimationAtATime;

	// Load From File
	void	AddAnimation(const std::string& element, const std::string& name);
	Frame	CreateFrame(const std::string& frame) const;
};

