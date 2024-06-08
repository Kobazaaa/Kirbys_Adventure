#pragma once
#include "SoundStream.h"
#include "SoundEffect.h"
#include <vector>

class SoundManager final
{
public:
	struct SoundStreamObj
	{
		explicit SoundStreamObj(const std::string& name, const std::string& filepath);
		SoundStreamObj(const SoundStreamObj& other);
		SoundStreamObj(SoundStreamObj&& other) noexcept;
		SoundStreamObj& operator=(const SoundStreamObj& rhs) = delete;
		SoundStreamObj& operator=(SoundStreamObj&& rhs) = delete;
		~SoundStreamObj();

		std::string m_Name;
		SoundStream* m_pSoundStream;
		std::string m_FilePath;

	};
	struct SoundEffectObj
	{
		explicit SoundEffectObj(const std::string& name, const std::string& filepath);
		SoundEffectObj(const SoundEffectObj& other);
		SoundEffectObj(SoundEffectObj&& other) noexcept;
		SoundEffectObj& operator=(const SoundEffectObj& rhs) = delete;
		SoundEffectObj& operator=(SoundEffectObj&& rhs) = delete;
		~SoundEffectObj();

		std::string m_Name;
		SoundEffect* m_pSoundEffect;
		std::string m_FilePath;
	};


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~			BEHAVIOURAL			~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool LoadSoundStream(const std::string& name, const std::string& filePath);
	static bool LoadSoundEffect(const std::string& name, const std::string& filePath);
	static void DeleteAllSound() noexcept;

	static void PlayStream(const std::string& name, bool loop = false);
	static void PlayEffect(const std::string& name, int loops = 0);

	static bool IsPlayingStream(const std::string& name);
	static bool IsPlayingEffect(const std::string& name);
	static void StopEffect(const std::string& name);

	static void ResetStream();
	static void PauseAll();
	static void ResumeAll();
	static void StopAll();

	static void SetVolume(int volume);
private:
	// Private Accessors
	static bool IsStreamLoaded(const std::string& name);
	static bool IsEffectloaded(const std::string& name);

	// Private Data Members
	static std::vector<SoundStreamObj*> m_vSoundStreams;
	static std::vector<SoundEffectObj*> m_vSoundEffects;

	static std::string m_CurrentActiveStream;
};

