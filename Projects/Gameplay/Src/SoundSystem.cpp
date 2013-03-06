#include "SoundSystem.h"
#include "SoundComponent.h"
#include <SoundWrapper.h>

SoundSystem::SoundSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::SoundSystem, 1, ComponentType::SoundComponent)
{
	m_audioBackend = p_audioBackend;
}

SoundSystem::~SoundSystem()
{

}

void SoundSystem::initialize()
{

}

void SoundSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++){

		SoundComponent* soundComp = static_cast<SoundComponent*>
			(p_entities[i]->getComponent(ComponentType::SoundComponent));
		updateSoundStatus(soundComp);
	}
}

void SoundSystem::inserted( Entity* p_entity )
{
	SoundComponent* soundComp = static_cast<SoundComponent*>
		(p_entity->getComponent(ComponentType::SoundComponent));

	//Create all the new sounds using the soundwrapper
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < soundComp->m_sounds[i].size(); j++){
			unsigned soundIndex = m_audioBackend->getSoundWrapper()->createSoundFromHeader
				(soundComp->m_sounds[i][j]);

			soundComp->m_sounds[i][j]->soundIndex = soundIndex;
		}
	}
}

void SoundSystem::removed( Entity* p_entity )
{
	SoundComponent* soundComp = static_cast<SoundComponent*>
		(p_entity->getComponent(ComponentType::SoundComponent));

	//Create all the new sounds using the soundwrapper
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < soundComp->m_sounds[i].size(); j++){
			AudioHeader* header = soundComp->m_sounds[i][j];
			m_audioBackend->getSoundWrapper()->destroySound(header->soundIndex);
		}
	}
}

void SoundSystem::updateSoundStatus( const SoundComponent* p_soundComponent )
{
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < p_soundComponent->m_sounds[i].size(); j++){
			AudioHeader* header = p_soundComponent->m_sounds[i][j];
			m_audioBackend->getSoundWrapper()->
				getSound(header->soundIndex)->setFrequency(header->frequency);

			if(header->playInterval == AudioHeader::TIMERBASED){
				header->timeSinceLastPlayed += m_world->getDelta();

				if(header->timeSinceLastPlayed >= header->timerInterval){
					m_audioBackend->changeAudioInstruction(header->soundIndex, 
					AudioHeader::PlayState::RESTART);
					header->timeSinceLastPlayed = 0;
				}
			}

			else if( header->playingState != header->queuedPlayingState ){
				header->playingState = header->queuedPlayingState;
				m_audioBackend->changeAudioInstruction(header->soundIndex, 
					header->playingState);
			}
		}
	}
}
