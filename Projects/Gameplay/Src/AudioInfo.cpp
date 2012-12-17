#include "AudioInfo.h"


AudioInfo::AudioInfo(int p_soundId, bool p_isPositionalSound)
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
