#include "AudioBackendSystem.h"

AudioBackendSystem::AudioBackendSystem() : EntitySystem(SystemType::AudioBackendSystem)
{
	m_soundWrapper = new SoundWrapper();
}

AudioBackendSystem::~AudioBackendSystem()
{
	delete m_soundWrapper;
}

void AudioBackendSystem::initialize()
{

}

void AudioBackendSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void AudioBackendSystem::updateListener( const SoundSceneInfo& p_listenerInfo )
{
	m_soundWrapper->updateListener(p_listenerInfo);
}

int AudioBackendSystem::createNonPositionalSound( const char* p_filePath )
{
	return m_soundWrapper->createNewNonPositionalSound(p_filePath);
}

int AudioBackendSystem::createPositionalSound( const char* p_filePath, 
											  const AglVector3& p_pos )
{
	return m_soundWrapper->createNewPositionalSound(p_filePath,p_pos);
}

void AudioBackendSystem::changeAudioInstruction( int p_index, 
												const SoundEnums::Instructions& p_instruction )
{
	m_soundWrapper->updateSound(p_index,p_instruction);
}
