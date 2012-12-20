#include "AudioBackendSystem.h"
#include <ToString.h>
#include <SoundWrapper.h>

AudioBackendSystem* AudioBackendSystem::m_selfPointer = NULL;

AudioBackendSystem::AudioBackendSystem() : EntitySystem(SystemType::AudioBackendSystem)
{
	m_soundWrapper = new SoundWrapper();
	m_selfPointer = this;
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

int AudioBackendSystem::createAmbientSound(BasicSoundCreationInfo* p_info)
{
	int index = m_soundWrapper->createAmbientSound(p_info);

	/************************************************************************/
	/* DEBUG INFO!															*/
	/************************************************************************/
	string temp = m_label + toString(index) +" "+ p_info->file;
	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(),temp.c_str(),
		stopOrPlaySound, (void*)index,"group=Ambient_Sound");
		//-END-

	return index;
}

int AudioBackendSystem::createPositionalSound(BasicSoundCreationInfo* p_info,
											  PositionalSoundCreationInfo* p_positionalInfo )
{
	int index = m_soundWrapper->createNewPositionalSound(p_info,p_positionalInfo);

	/************************************************************************/
	/* DEBUG INFO!															*/
	/************************************************************************/
	string temp = m_label + toString(index) + " " + p_info->file;

	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(), temp.c_str(),
		stopOrPlaySound, (void*)index, "group=Positional_Sound");

	temp = "";
	temp =  toString(index) + " " + "Left";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(temp.c_str(),TW_TYPE_FLOAT,
		m_soundWrapper->getSound(index)->getLeftChannelRef(),"group=Positional_Sound");

	temp = "";
	temp = toString(index) + " " + "Right";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(temp.c_str(),TW_TYPE_FLOAT,
		m_soundWrapper->getSound(index)->getRightChannelRef(),"group=Positional_Sound");

	//-END-

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
	if(m_selfPointer->m_soundWrapper->isPlaying((int)p_clientData))
		m_selfPointer->m_soundWrapper->updateSound((int)p_clientData,SoundEnums::STOP);
	else
		m_selfPointer->m_soundWrapper->updateSound((int)p_clientData,SoundEnums::PLAY);
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
