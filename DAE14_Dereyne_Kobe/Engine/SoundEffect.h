#pragma once
#include <string>
class Mix_Chunk;
class SoundEffect final
{
public:
	explicit SoundEffect( const std::string& path );
	~SoundEffect( );
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(const SoundEffect& rhs) = delete;
	SoundEffect( SoundEffect&& other) = delete;
	SoundEffect& operator=( SoundEffect&& rhs) = delete;

	bool IsLoaded( ) const;
	bool Play( const int loops );

	void Stop();
	bool IsPlaying();
	int GetChannel() const;

	void SetVolume( const int value ); 
	int GetVolume( ) const; 
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	int m_Channel{-1};

	Mix_Chunk* m_pMixChunk;
};
