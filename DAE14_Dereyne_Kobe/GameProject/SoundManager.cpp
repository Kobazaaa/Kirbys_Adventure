#include "pch.h"
#include "utils.h"
#include <iostream>
#include "SoundManager.h"

std::vector<SoundManager::SoundStreamObj*> SoundManager::m_vSoundStreams;
std::vector<SoundManager::SoundEffectObj*> SoundManager::m_vSoundEffects;

bool SoundManager::LoadSoundStream(const std::string& name, const std::string& filePath)
{
	if (!IsStreamLoaded(name))
	{
		m_vSoundStreams.push_back(std::move(new SoundStreamObj{ name, filePath }));

		if (!m_vSoundStreams.at(m_vSoundStreams.size() - 1)->m_pSoundStream->IsLoaded())
		{
			std::cout << "SOUND_MANAGER: Could not load SoundStream with name \"" << name << "\" and filepath \"" << filePath << "\" correctly\n";
			return false;
		}
	}

	if (utils::DEBUG_MODE) std::cout << m_vSoundStreams.size() << "\t Stream\t" << name << "\t" << filePath << std::endl;

	return true;
}

bool SoundManager::LoadSoundEffect(const std::string& name, const std::string& filePath)
{
	if (!IsEffectloaded(name))
	{
		m_vSoundEffects.push_back(std::move(new SoundEffectObj{ name, filePath }));

		if (!m_vSoundEffects.at(m_vSoundEffects.size() - 1)->m_pSoundEffect->IsLoaded())
		{
			std::cout << "SOUND_MANAGER: Could not load SoundEffect with name \"" << name << "\" and filepath \"" << filePath << "\" correctly\n";
			return false;
		}
	}

	std::cout << m_vSoundEffects.size() << "\t Effect" << std::endl;

	return true;
}

void SoundManager::DeleteAllSound() noexcept
{
	for (int index{}; index < m_vSoundEffects.size(); ++index)
	{
		delete m_vSoundEffects[index];
		m_vSoundEffects[index] = nullptr;
	}
	for (int index{}; index < m_vSoundStreams.size(); ++index)
	{
		delete m_vSoundStreams[index];
		m_vSoundStreams[index] = nullptr;
	}
}

void SoundManager::PlayStream(const std::string& name, bool loop)
{
	for (int index{}; index < m_vSoundStreams.size(); ++index)
	{
		if (m_vSoundStreams[index]->m_Name == name)
		{
			m_vSoundStreams[index]->m_pSoundStream->Play(loop);
		}
	}
}

void SoundManager::PlayEffect(const std::string& name, int loops)
{
	for (int index{}; index < m_vSoundEffects.size(); ++index)
	{
		if (m_vSoundEffects[index]->m_Name == name)
		{
			m_vSoundEffects[index]->m_pSoundEffect->Play(loops);
		}
	}
}

bool SoundManager::IsPlayingStream(const std::string& name)
{
	for (int index{}; index < m_vSoundStreams.size(); ++index)
	{
		if (m_vSoundStreams[index]->m_Name == name)
		{
			return (m_vSoundStreams[index]->m_pSoundStream->IsPlaying());
		}
	}
	return false;
}

bool SoundManager::IsPlayingEffect(const std::string& name)
{
	for (int index{}; index < m_vSoundEffects.size(); ++index)
	{
		if (m_vSoundEffects[index]->m_Name == name)
		{
			return m_vSoundEffects[index]->m_pSoundEffect->IsPlaying();
		}
	}
	return false;
}

void SoundManager::StopEffect(const std::string& name)
{
	for (int index{}; index < m_vSoundEffects.size(); ++index)
	{
		if (m_vSoundEffects[index]->m_Name == name)
		{
			m_vSoundEffects[index]->m_pSoundEffect->Stop();
		}
	}
}

void SoundManager::PauseAll()
{
	SoundEffect::PauseAll();
	SoundStream::Pause();
}

void SoundManager::ResumeAll()
{
	SoundEffect::ResumeAll();
	SoundStream::Resume();
}

void SoundManager::StopAll()
{
	SoundEffect::StopAll();
	SoundStream::Stop();
}


bool SoundManager::IsStreamLoaded(const std::string& name)
{
	for (int index{}; index < m_vSoundStreams.size(); ++index)
	{
		if (m_vSoundStreams[index]->m_Name == name)
		{
			std::cout << "SOUND_MANAGER: Another SoundStream has already been created with the name \"" << name << "\", please try again with another name\n";
			return true;
		}
	}
	return false;
}

bool SoundManager::IsEffectloaded(const std::string& name)
{
	for (int index{}; index < m_vSoundEffects.size(); ++index)
	{
		if (m_vSoundEffects[index]->m_Name == name)
		{
			std::cout << "SOUND_MANAGER: Another SoundStream has already been created with the name \"" << name << "\", please try again with another name\n";
			return true;
		}
	}
	return false;
}

#pragma region SoundStreamObject
SoundManager::SoundStreamObj::SoundStreamObj(const std::string& name, const std::string& filepath)
	: m_Name			{ name }
	, m_pSoundStream	{ new SoundStream(filepath)}
	, m_FilePath		{ filepath }
{
}

SoundManager::SoundStreamObj::SoundStreamObj(const SoundStreamObj& other)
{
	delete this->m_pSoundStream;

	this->m_Name			=		other.m_Name;
	this->m_pSoundStream	=		new SoundStream(other.m_FilePath);
	this->m_FilePath		=		other.m_FilePath;
}

SoundManager::SoundStreamObj::SoundStreamObj(SoundStreamObj&& other) noexcept
{
	delete this->m_pSoundStream;

	this->m_Name			=		std::move(other.m_Name);
	this->m_FilePath		=		std::move(other.m_FilePath);
	this->m_pSoundStream	=		std::move(other.m_pSoundStream);

	other.m_pSoundStream	=		nullptr;
}

SoundManager::SoundStreamObj::~SoundStreamObj()
{
	delete m_pSoundStream;
	m_pSoundStream = nullptr;

	std::cout << m_Name << "\t Stream" << std::endl;
}
#pragma endregion
#pragma region SoundEffectObject
SoundManager::SoundEffectObj::SoundEffectObj(const std::string& name, const std::string& filepath)
	: m_Name			{ name }
	, m_pSoundEffect	{ new SoundEffect(filepath) }
	, m_FilePath		{ filepath }
{
}

SoundManager::SoundEffectObj::SoundEffectObj(const SoundEffectObj& other)
{
	delete this->m_pSoundEffect;

	this->m_Name			=		other.m_Name;
	this->m_pSoundEffect	=		new SoundEffect(other.m_FilePath);
	this->m_FilePath		=		other.m_FilePath;
}

SoundManager::SoundEffectObj::SoundEffectObj(SoundEffectObj&& other) noexcept
{
	delete this->m_pSoundEffect;

	this->m_Name			=		std::move(other.m_Name);
	this->m_FilePath		=		std::move(other.m_FilePath);
	this->m_pSoundEffect	=		std::move(other.m_pSoundEffect);

	other.m_pSoundEffect	=		nullptr;
}

SoundManager::SoundEffectObj::~SoundEffectObj()
{
	delete m_pSoundEffect;
	m_pSoundEffect = nullptr;

	std::cout << m_Name << "\t Effect" << std::endl;
}
#pragma endregion