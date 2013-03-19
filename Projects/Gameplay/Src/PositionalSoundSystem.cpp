#include "PositionalSoundSystem.h"

#include <Sound.h>
#include <PositionalSound.h>
#include <SoundWrapper.h>

#include "AudioBackendSystem.h"

#include "Transform.h"
#include "AudioInfo.h"
#include "SoundComponent.h"
#include <Globals.h>
#include <PositionalSoundCreationInfo.h>
#include <AudioHeader.h>
#include "AudioListenerSystem.h"
#include <ValueClamp.h>
#include "MeshOffsetTransform.h"

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

	MeshOffsetTransform* offsetTrans = static_cast<MeshOffsetTransform*>(
		p_entity->getComponent( ComponentType::MeshOffsetTransform ));

	SoundOrientation* listener = m_audioListenerSystem->getListenerOrientation();
	for (unsigned int i = 0; i < ambientRange->size(); i++){
		
		AudioHeader* header = ambientRange->at(i);
		AglVector3 posw = header->pos;
		AglMatrix soundTransform= AglMatrix::createTranslationMatrix(posw);
		soundTransform*=trans->getMatrix(); // fix: local to world space (Jarl 11-03-2013)
		posw = soundTransform.GetTranslation();

		AglVector3 distanceVec = listener->listenerPos - posw;
		float length = AglVector3::length(distanceVec);
		
		float newVolume=0.0f;
		//Check if sound is within the inner circle
		/*
		This seems to be incorrect:
		if(header->minRange > 0 && length < header->minRange)
			newVolume = 1.0f;
		else{
			//Calculate the falloff volume
			newVolume = (header->maxRange - length)/header->maxRange;
			if(newVolume<0){
				newVolume = 0;
			}
		}
		not correct falloff from min- to max-range. 
		Test with the difference between length=minRange-0.01f and length=minrange
		(Jarl 11-03-2013) Replacing it with this:*/
		float min = header->minRange;
		float max = header->maxRange;
		newVolume = (1.0f-((clamp(length, min, max+min)-min)/max))*header->volume;

		m_audioBackendSystem->getSoundWrapper()->getSound(header->soundIndex)->
			getSourceVoice()->SetVolume(newVolume);
	}

	vector<AudioHeader*>* positionalSound = 
		p_soundComponent->getAllAudiosByType(AudioHeader::POSITIONALSOUND);

	for (unsigned int i = 0; i < positionalSound->size(); i++){
		AudioHeader* header = positionalSound->at(i);

		AglMatrix worldTransform; 
		if(offsetTrans){
			worldTransform = offsetTrans->offset.inverse() * trans->getMatrix();
		}
		else{
			worldTransform = trans->getMatrix();
		}
		
		header->pos = worldTransform.GetTranslation();
		header->front = worldTransform.GetForward();
		header->top = worldTransform.GetUp();

		//posw.transform(trans->getMatrix());  // fix: local to world space (Jarl 11-03-2013)

		PositionalSound* sound = static_cast<PositionalSound*>
			(m_audioBackendSystem->getSoundWrapper()->getSound(header->soundIndex));
		sound->updateEmitter(header);
		m_audioBackendSystem->updateOutputMatrix(header->soundIndex);
	}
	
	
}
