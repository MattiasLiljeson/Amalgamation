#include "AudioBackendSystem.h"

AudioBackendSystem* AudioBackendSystem::m_theSystem = NULL;

AudioBackendSystem::AudioBackendSystem() : EntitySystem(SystemType::AudioBackendSystem)
{
	m_soundWrapper = new SoundWrapper();
	m_theSystem = this;
	m_label = "Play/Stop: ";
}

AudioBackendSystem::~AudioBackendSystem()
{
	delete m_soundWrapper;
}

void AudioBackendSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		"Master Volume",TwType::TW_TYPE_FLOAT,m_soundWrapper->getMasterVolumeRef(),
		"group=Overall min=0 max=10 step=0.001 precision=3");
}

void AudioBackendSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void AudioBackendSystem::updateListener( const SoundOrientation& p_listenerInfo )
{
	m_soundWrapper->updateListener(p_listenerInfo);
	m_soundWrapper->updateMasterVolume();
}

int AudioBackendSystem::createAmbientSound(BasicSoundCreationInfo* p_info)
{
	int index = m_soundWrapper->createAmbientSound(p_info);
	string temp = m_label + toString(index);
	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(),temp.c_str(),
		stopOrPlaySound, (void*)index,"group=Ambient_Sound");

	return index;
}

int AudioBackendSystem::createPositionalSound( BasicSoundCreationInfo* p_info, 
											  const AglVector3& p_pos )
{
	int index = m_soundWrapper->createNewPositionalSound(p_info,p_pos);

	string temp = m_label + toString(index);
	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(), temp.c_str(),
		stopOrPlaySound, (void*)index, "group=Positional_Sound");

	return index;
}

void AudioBackendSystem::changeAudioInstruction( int p_index, 
												const SoundEnums::Instructions& p_instruction )
{
	m_soundWrapper->updateSound(p_index,p_instruction);
}

SoundWrapper* AudioBackendSystem::getSoundWrapper()
{
	return m_soundWrapper;
}

void TW_CALL AudioBackendSystem::stopOrPlaySound( void* p_clientData )
{
	if(m_theSystem->m_soundWrapper->isPlaying((int)p_clientData))
		m_theSystem->m_soundWrapper->updateSound((int)p_clientData,SoundEnums::STOP);
	else
		m_theSystem->m_soundWrapper->updateSound((int)p_clientData,SoundEnums::PLAY);
}
