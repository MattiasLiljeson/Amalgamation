#include <Sound.h>
#include <PositionalSound.h>
#include <SoundWrapper.h>

#include "PositionalSoundSystem.h"
#include "AudioBackendSystem.h"

#include "Transform.h"
#include "AudioInfo.h"
#include "SoundComponent.h"
#include <Globals.h>
#include <PositionalSoundCreationInfo.h>
#include <AudioHeader.h>
#include "AudioListenerSystem.h"

PositionalSoundSystem::PositionalSoundSystem()
	: EntitySystem( SystemType::PositionalSoundSystem, 2,
	ComponentType::SoundComponent, ComponentType::Transform )
{
}

PositionalSoundSystem::~PositionalSoundSystem()
{
}

void PositionalSoundSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++){

		SoundComponent* soundComp = static_cast<SoundComponent*>
			(p_entities[i]->getComponent(ComponentType::SoundComponent));
		processSoundComponent(p_entities[i], soundComp);
	}
}

void PositionalSoundSystem::initialize()
{
	m_audioBackendSystem = static_cast<AudioBackendSystem*>
		(m_world->getSystem(SystemType::AudioBackendSystem ));

	m_audioListenerSystem = static_cast<AudioListenerSystem*>
		(m_world->getSystem(SystemType::AudioListenerSystem));
}

void PositionalSoundSystem::processSoundComponent(Entity* p_entity, 
												  SoundComponent* p_soundComponent)
{
	Transform* trans = static_cast<Transform*>
		(p_entity->getComponent(ComponentType::Transform));

	vector<AudioHeader*>* ambientRange = 
		p_soundComponent->getAllAudiosByType(AudioHeader::AMBIENTRANGE);
	
	SoundOrientation* listener = m_audioListenerSystem->getListenerOrientation();
	for (unsigned int i = 0; i < ambientRange->size(); i++){
		
		AudioHeader* header = ambientRange->at(i);
		header->pos = trans->getTranslation();

		AglVector3 distanceVec = listener->listenerPos - header->pos;
		float length = AglVector3::length(distanceVec);
		
		float newVolume;
		//Check if sound is within the inner circle
		if(header->minRange > 0 && length < header->minRange)
			newVolume = 1.0f;
		else{
			//Calculate the falloff volume
			newVolume = (header->maxRange - length)/header->maxRange;
			if(newVolume<0){
				newVolume = 0;
			}
		}

		m_audioBackendSystem->getSoundWrapper()->getSound(header->soundIndex)->
			getSourceVoice()->SetVolume(newVolume);
	}

	vector<AudioHeader*>* positionalSound = 
		p_soundComponent->getAllAudiosByType(AudioHeader::POSITIONALSOUND);

	for (unsigned int i = 0; i < positionalSound->size(); i++){
	}
	
	
}
