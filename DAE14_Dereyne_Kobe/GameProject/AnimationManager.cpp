#include "pch.h"
#include "AnimationManager.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

#pragma region AnimObj
AnimObj::AnimObj(const std::string& name, Animation* animation)
	: m_Name{ name }
	, m_pAnimation{ animation }
{
}
AnimObj::AnimObj(AnimObj&& other) noexcept
{
	this->m_Name		= std::move(other.m_Name);
	this->m_pAnimation	= std::move(other.m_pAnimation);

	other.m_pAnimation = nullptr;
}
AnimObj& AnimObj::operator=(AnimObj&& rhs) noexcept
{
	if (this != &rhs)
	{
		this->m_Name		= std::move(rhs.m_Name);

		delete this->m_pAnimation;
		this->m_pAnimation	= std::move(rhs.m_pAnimation);

		rhs.m_pAnimation = nullptr;
	}
	
	return *this;
}
AnimObj::~AnimObj() noexcept
{
	delete m_pAnimation;
	m_pAnimation = nullptr;
}
#pragma endregion

#pragma region AnimationManager
AnimationManager::AnimationManager(Texture* spritesheet, bool oneAnimationAtATime)
	: m_pSpriteSheet{ spritesheet }
	, m_OneAnimationAtATime{oneAnimationAtATime}
{
}
AnimationManager::AnimationManager(const std::string& textureName, bool oneAnimationAtATime)
	: m_pSpriteSheet{ TextureManager::GetTexture(textureName) }
	, m_OneAnimationAtATime {oneAnimationAtATime}
{
}
AnimationManager::AnimationManager(AnimationManager&& other) noexcept
{
	this->m_vAnimations			= std::move(other.m_vAnimations);
	this->m_pSpriteSheet		= std::move(other.m_pSpriteSheet);
	other.m_pSpriteSheet		= nullptr;
	this->m_OneAnimationAtATime = std::move(other.m_OneAnimationAtATime);
}
AnimationManager& AnimationManager::operator=(AnimationManager&& rhs) noexcept
{
	if (this != &rhs)
	{
		this->m_vAnimations				= std::move(rhs.m_vAnimations);
		this->m_OneAnimationAtATime		= std::move(rhs.m_OneAnimationAtATime);

		delete this->m_pSpriteSheet;
		this->m_pSpriteSheet			= std::move(rhs.m_pSpriteSheet);

		rhs.m_pSpriteSheet				= nullptr;
	}

	return *this;
}
AnimationManager::~AnimationManager() noexcept
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		delete m_vAnimations[index];
		m_vAnimations[index] = nullptr;
	}
}

void AnimationManager::Add(const std::string& name, Animation* animation)
{
	m_vAnimations.push_back(new AnimObj(name, animation));
}
void AnimationManager::LoadFromFile(const std::string& filePath)
{
	std::ifstream inputFile{ filePath };
	if (!inputFile)
	{
		std::cout << "Could not properly open the following file: " << filePath << "\n Please try again.\n";
		assert(inputFile);
	}
	else
	{
		std::stringstream sstream;
		std::string line;
		std::string element;
		while (std::getline(inputFile, line))
		{
			if (line != "\n" and line != "")
			{
				sstream << line << "\n";

				std::string tag;
				if (line[0] == '<')
				{
					tag = line.substr(1, line.find(">") - 1);
				}
				if (tag[0] == '/')
				{
					element = sstream.str();
					sstream.str("");

					AddAnimation(element, tag.substr(1));
				}
			}
		}
	}
}

void AnimationManager::Update(float elapsedSec, const std::string& animName)
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		if (m_vAnimations[index]->m_Name == animName)
		{
			m_vAnimations[index]->m_pAnimation->Activate();
		}
		else if (m_OneAnimationAtATime) m_vAnimations[index]->m_pAnimation->Deactivate();

		m_vAnimations[index]->m_pAnimation->Update(elapsedSec);
	}
}
void AnimationManager::Draw(const Point2f& position) const
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		m_vAnimations[index]->m_pAnimation->Draw(m_pSpriteSheet, position);
	}
}

void AnimationManager::Deactivate(const std::string& animName)
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		if (m_vAnimations[index]->m_Name == animName)
		{
			m_vAnimations[index]->m_pAnimation->Deactivate();
		}
	}
}

bool AnimationManager::IsActive(const std::string& animName) const
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		if (m_vAnimations[index]->m_Name == animName)
		{
			return m_vAnimations[index]->m_pAnimation->IsActive();
		}
	}
	
	return false;
}

bool AnimationManager::IsDone(const std::string& animName) const
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		if (m_vAnimations[index]->m_Name == animName)
		{
			return m_vAnimations[index]->m_pAnimation->IsDone();
		}
	}

	return false;
}

int AnimationManager::GetCurrentFrame(const std::string& animName) const
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		if (m_vAnimations[index]->m_Name == animName)
		{
			return m_vAnimations[index]->m_pAnimation->GetCurrentFrame();
		}
	}

	return -1;
}

void AnimationManager::AddAnimation(const std::string& element, const std::string& name)
{
	std::stringstream elementStringStream;
	elementStringStream << element;

	std::vector<Frame> vFrames;
	bool loop{false};

	std::stringstream sstream;
	std::string line;
	std::string frame;
	while (std::getline(elementStringStream, line))
	{
		if (line != "\n" and line != "" and line.find(name) == std::string::npos)
		{
			sstream << line << "\n";

			std::string tag;
			size_t frameStartPos{ line.find("<") };
			if (frameStartPos != std::string::npos)
			{
				tag = line.substr(frameStartPos + 1, line.find(">") - frameStartPos - 1);
			}
			if (tag[0] == '/')
			{
				frame = sstream.str();
				sstream.str("");

				vFrames.push_back(CreateFrame(frame));
			}
		}

		if (line.find("loop") != std::string::npos)
		{
			loop = utils::ToBool(utils::GetAttributeValue("loop", line));
		}
	}

	m_vAnimations.push_back(new AnimObj(name, new Animation(std::move(vFrames), loop)));
}
Frame AnimationManager::CreateFrame(const std::string& frame) const
{
	Rectf tempRect{};
	float tempDuration{};

	tempRect.left = utils::ToPoint2f(utils::GetAttributeValue("position", frame)).x;
	tempRect.bottom = -utils::ToPoint2f(utils::GetAttributeValue("position", frame)).y;
	tempRect.width = utils::ToFloat(utils::GetAttributeValue("width", frame));
	tempRect.height = utils::ToFloat(utils::GetAttributeValue("height", frame));
	tempDuration = utils::ToFloat(utils::GetAttributeValue("duration", frame));

	return Frame(tempRect, tempDuration);
}
#pragma endregion