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
