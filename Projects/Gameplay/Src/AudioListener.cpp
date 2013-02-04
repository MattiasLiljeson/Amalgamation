#include "AudioListener.h"

AudioListener::AudioListener(float p_listenerVolume/* =1.0f */)
	: Component( ComponentType::AudioListener )
{
	m_listenerVolume = p_listenerVolume;
}

float* AudioListener::getMasterVolumeRef()
{
	return &m_listenerVolume;
}

float AudioListener::getListenerVolume() const
{
	return m_listenerVolume;
}

void AudioListener::setListenerVolume( float p_volume )
{
	m_listenerVolume = p_volume;
}