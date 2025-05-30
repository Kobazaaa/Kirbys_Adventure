#include "base.h"
#include <iostream>
#include "SoundEffect.h"

SoundEffect::SoundEffect( const std::string& path )
	:m_pMixChunk{ Mix_LoadWAV( path.c_str( ) ) }
{
	if ( m_pMixChunk == nullptr )
	{
		const std::string errorMsg = "SoundEffect: Failed to load " + path + ",\nSDL_mixer Error: " + Mix_GetError( );
		std::cerr << errorMsg;
	}
}
SoundEffect::~SoundEffect( )
{
	Mix_FreeChunk( m_pMixChunk );
	m_pMixChunk = nullptr;
}
bool SoundEffect::IsLoaded( ) const
{
	return m_pMixChunk != nullptr;
}

bool SoundEffect::Play( const int loops )
{
	// Don't save the channel as a data member, 
	// because when it stops playing the channel becomes free
	// and available for usage by other effects
	if ( m_pMixChunk != nullptr )
	{
		const int channel{ Mix_PlayChannel( -1, m_pMixChunk, loops ) };
		m_Channel = channel;

		return channel == -1 ? false : true;
	}
	else
	{
		return false;
	}
}
void SoundEffect::Stop()
{
	if (m_Channel != -1)
	{
		Mix_HaltChannel(m_Channel);
		m_Channel = -1;
	}
}
bool SoundEffect::IsPlaying()
{
	if (m_Channel != -1)
	{
		bool isPlaying{ Mix_Playing(m_Channel) != 0 };
		if (!isPlaying) m_Channel = -1;
		return isPlaying;
	}
	else return false;
}
int SoundEffect::GetChannel() const
{
	return m_Channel;
}

void SoundEffect::SetVolume( const int value )
{
	if ( m_pMixChunk != nullptr )
	{
		Mix_VolumeChunk( m_pMixChunk, value );
	}
}
int SoundEffect::GetVolume( ) const
{
	if ( m_pMixChunk != nullptr )
	{
		return Mix_VolumeChunk( m_pMixChunk, -1 );
	}
	else
	{
		return -1;
	}
}

void SoundEffect::StopAll( )
{
	Mix_HaltChannel(-1 );
}
void SoundEffect::PauseAll( )
{
	Mix_Pause( -1 );
}
void SoundEffect::ResumeAll( )
{
	Mix_Resume( -1 );
}




