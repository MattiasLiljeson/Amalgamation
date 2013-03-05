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

/*
int AudioBackendSystem::createAmbientSound(BasicSoundCreationInfo* p_info)
{
	int index = m_soundWrapper->createAmbientSound(p_info);

	if( AntTweakBarWrapper::getInstance() != NULL )
	{
		string temp = m_label + toString(index) +" "+ p_info->fileName;
		TwAddButton(AntTweakBarWrapper::getInstance()->getAntBar(AntTweakBarWrapper::SOUND),
			temp.c_str(), stopOrPlaySound, (void*)index,"group=Ambient_Sound");
			//-END-
	}
	else
	{
		//cast exception
		int korv = 0;
	}
	return index;
}
*/
/*
int AudioBackendSystem::createPositionalSound(BasicSoundCreationInfo* p_info,
											  PositionalSoundCreationInfo* p_positionalInfo )
{
	int index = m_soundWrapper->createNewPositionalSound(p_info,p_positionalInfo);

	string temp = m_label + toString(index) + " " + p_info->fileName;

	if( AntTweakBarWrapper::getInstance() != NULL )
	{

	TwAddButton(AntTweakBarWrapper::getInstance()->getAntBar(AntTweakBarWrapper::SOUND), 
		temp.c_str(), stopOrPlaySound, (void*)index, "group=Positional_Sound");

	temp = "";
	temp =  toString(index) + " " + "Left";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::SOUND,
		temp.c_str(),TW_TYPE_FLOAT, m_soundWrapper->getSound(index)->getLeftChannelRef(),
		"group=Positional_Sound");

	temp = "";
	temp = toString(index) + " " + "Right";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::SOUND,
		temp.c_str(),TW_TYPE_FLOAT,	m_soundWrapper->getSound(index)->getRightChannelRef(),
		"group=Positional_Sound");

	}
	else
	{
		//cast exception
		int korv = 0;
	}
	//-END-

	return index;
}
*/
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

/*
void AudioBackendSystem::playPositionalSoundEffect( string p_path, string p_filename,
												   AglVector3 p_position )
{
	BasicSoundCreationInfo creationalSoundInfo = BasicSoundCreationInfo(
		p_filename.c_str(),
		p_path.c_str(),
		false);
	PositionalSoundCreationInfo positionCreationalSoundInfo = PositionalSoundCreationInfo(
		p_position);
	int soundIndex = m_soundWrapper->createNewPositionalSound( &creationalSoundInfo,
		&positionCreationalSoundInfo );
}
*/
/*
void AudioBackendSystem::playSoundEffect(string p_path, string p_filename)
{
	BasicSoundCreationInfo creationalSoundInfo = BasicSoundCreationInfo(
		p_filename.c_str(),
		p_path.c_str(),
		false);
	int soundIndex = m_soundWrapper->createAmbientSound(&creationalSoundInfo);
}
*/