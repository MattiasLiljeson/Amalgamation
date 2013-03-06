#include "AudioBackendSystem.h"
#include <ToString.h>
#include <SoundWrapper.h>


AudioBackendSystem::AudioBackendSystem() : EntitySystem(SystemType::AudioBackendSystem)
{
	m_soundWrapper = new SoundWrapper();
	m_label = "P/S: ";
}

AudioBackendSystem::~AudioBackendSystem()
{
	delete m_soundWrapper;
}

void AudioBackendSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void AudioBackendSystem::updateListener( const SoundOrientation& p_listenerInfo )
{
	m_soundWrapper->updateListener(p_listenerInfo);
	m_soundWrapper->updateMasterVolume();
}

void AudioBackendSystem::changeAudioInstruction( unsigned int p_index, 
												const AudioHeader::PlayState& p_instruction )
{
	m_soundWrapper->updateSound(p_index,p_instruction);
}

SoundWrapper* AudioBackendSystem::getSoundWrapper()
{
	return m_soundWrapper;
}
void AudioBackendSystem::updateListenerVolume( float p_volume )
{
	m_soundWrapper->setMasterVolume(p_volume);
	m_soundWrapper->updateMasterVolume();
}

void AudioBackendSystem::updateOutputMatrix( int p_index )
{
	m_soundWrapper->updateOutputMatrix(p_index);
}