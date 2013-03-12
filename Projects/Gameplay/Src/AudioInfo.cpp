#include "AudioInfo.h"

AudioInfo::AudioInfo()
	: Component( ComponentType::AudioInfo )
{
	m_soundId=-1;
	m_isPositionalSound=true;
}

AudioInfo::AudioInfo(int p_soundId, bool p_isPositionalSound)
	: Component( ComponentType::AudioInfo )
{
	m_isPositionalSound = p_isPositionalSound;
	m_soundId			= p_soundId;
}

AudioInfo::~AudioInfo()
{
}

int AudioInfo::getSoundId() const
{
	return m_soundId;
}

bool AudioInfo::isPositionalSound() const
{
	return m_isPositionalSound;
}
