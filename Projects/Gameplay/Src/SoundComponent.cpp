#include "SoundComponent.h"


SoundComponent::SoundComponent() : Component(ComponentType::SoundComponent)
{
	for (unsigned int i=0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		vector<AudioHeader*> audioVector;
		m_sounds.push_back(audioVector);
	}
}

SoundComponent::~SoundComponent()
{
	for(unsigned int i=0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for (unsigned int j = 0; j < m_sounds.at(i).size(); j++){
			delete m_sounds.at(i).at(j);
		}
	}
}
void SoundComponent::addAudioHeader(AudioHeader* p_audioHeader){
	m_sounds.at(p_audioHeader->soundType).push_back(p_audioHeader);
}

vector<AudioHeader*>* SoundComponent::getAllAudiosByType( AudioHeader::SoundType p_soundType )
{
	return &m_sounds.at(p_soundType);
}

AudioHeader* SoundComponent::getSoundHeaderByName( AudioHeader::SoundType p_soundType, 
												  const char* p_name )
{
	for (unsigned int i = 0; i < m_sounds.at(p_soundType).size(); i++){
		if(p_name == m_sounds[p_soundType][i]->getSoundName())
			return m_sounds[p_soundType][i];
	}

	return NULL;
}

unsigned int SoundComponent::getSoundIndexByName( AudioHeader::SoundType p_soundType, 
											 const char* p_name )
{
	for (unsigned int i = 0; i < m_sounds.at(p_soundType).size(); i++){
		if(p_name == m_sounds[p_soundType][i]->getSoundName())
			return i;
	}

	return -1;
}

AudioHeader* SoundComponent::getSoundHeaderByIndex( AudioHeader::SoundType p_soundType, 
												   unsigned int p_index )
{
	return m_sounds[p_soundType][p_index];
}

void SoundComponent::setSoundHeadersPlayingState(AudioHeader::PlayState p_playState, 
												 AudioHeader::SoundType p_soundType, 
												 const char* p_name)
{
	getSoundHeaderByName(p_soundType,p_name)->queuedPlayingState = p_playState;
}
