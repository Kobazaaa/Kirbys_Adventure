#include "pch.h"
#include "AnimationManager.h"
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
AnimationManager::AnimationManager(Texture* spritesheet)
	: m_pSpriteSheet{ spritesheet }
{
}
AnimationManager::AnimationManager(const std::string& textureName)
	: m_pSpriteSheet{ TextureManager::GetTexture(textureName) }
{
}
AnimationManager::AnimationManager(AnimationManager&& other) noexcept
{
	this->m_vAnimations		= std::move(other.m_vAnimations);
	this->m_pSpriteSheet	= std::move(other.m_pSpriteSheet);
	other.m_pSpriteSheet	= nullptr;
}
AnimationManager& AnimationManager::operator=(AnimationManager&& rhs) noexcept
{
	if (this != &rhs)
	{
		this->m_vAnimations		= std::move(rhs.m_vAnimations);

		delete this->m_pSpriteSheet;
		this->m_pSpriteSheet	= std::move(rhs.m_pSpriteSheet);

		rhs.m_pSpriteSheet		= nullptr;
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
		else m_vAnimations[index]->m_pAnimation->Deactivate();

		m_vAnimations[index]->m_pAnimation->Update(elapsedSec);
	}
}
void AnimationManager::Draw(const Point2f& position, const std::string& animName) const
{
	for (int index{}; index < m_vAnimations.size(); index++)
	{
		m_vAnimations[index]->m_pAnimation->Draw(m_pSpriteSheet, position);
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
			loop = ToBool(GetAttributeValue("loop", line));
		}
	}

	m_vAnimations.push_back(new AnimObj(name, new Animation(std::move(vFrames), loop)));
}
Frame AnimationManager::CreateFrame(const std::string& frame) const
{
	Rectf tempRect{};
	float tempDuration{};

	tempRect.left = ToPoint2f(GetAttributeValue("position", frame)).x;
	tempRect.bottom = -ToPoint2f(GetAttributeValue("position", frame)).y;
	tempRect.width = ToFloat(GetAttributeValue("width", frame));
	tempRect.height = ToFloat(GetAttributeValue("height", frame));
	tempDuration = ToFloat(GetAttributeValue("duration", frame));

	return Frame(tempRect, tempDuration);
}

Point2f AnimationManager::ToPoint2f(const std::string& point2fString) const
{
	Point2f point;

	std::stringstream stringstream{};
	stringstream << point2fString;
	std::string temp;

	std::getline(stringstream, temp, ',');
	point.x = std::stof(temp);
	std::getline(stringstream, temp, ',');
	point.y = std::stof(temp);;

	return point;
}
float AnimationManager::ToFloat(const std::string& floatString) const
{
	return std::stof(floatString);
}
bool AnimationManager::ToBool(const std::string& booleanString) const
{
	if (booleanString == "true") return true;
	else return false;
}
std::string AnimationManager::GetAttributeValue(const std::string& attrName, const std::string& element) const
{
	std::string attribute;
	attribute = attrName;

	int pos{ static_cast<int>(element.find(attribute) + attrName.length() + 2) };
	int length{ static_cast<int>(element.find('"', pos) - pos) };

	attribute = element.substr(pos, length);

	return attribute;
}
#pragma endregion