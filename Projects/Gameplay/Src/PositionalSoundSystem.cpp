#include <Sound.h>
#include <PositionalSound.h>
#include <SoundWrapper.h>

#include "PositionalSoundSystem.h"
#include "AudioBackendSystem.h"

#include "Transform.h"
#include "AudioInfo.h"
#include "PositionalSoundEffect.h"
#include <Globals.h>
#include <PositionalSoundCreationInfo.h>

PositionalSoundSystem::PositionalSoundSystem()
	: EntitySystem( SystemType::PositionalSoundSystem, 2,
	ComponentType::PositionalSoundEffect, ComponentType::Transform )
{
}

PositionalSoundSystem::~PositionalSoundSystem()
{
}

void PositionalSoundSystem::processEntities( const vector<Entity*>& p_entities )
{
	updateSoundPositions(p_entities);
}

void PositionalSoundSystem::initialize()
{
	m_audioBackendSystem = static_cast<AudioBackendSystem*>(m_world->getSystem(
		SystemType::AudioBackendSystem ));
}

void PositionalSoundSystem::inserted( Entity* p_entity )
{
	PositionalSoundEffect* positionalSoundEffect = NULL;
	Transform* transform = NULL;
	positionalSoundEffect = static_cast<PositionalSoundEffect*>(p_entity->getComponent(
		ComponentType::AudioInfo));
	transform = static_cast<Transform*>(p_entity->getComponent(
		ComponentType::Transform));

	BasicSoundCreationInfo creationalSoundInfo = BasicSoundCreationInfo(
		"engine-noise.wav", TESTSOUNDEFFECTPATH.c_str(), positionalSoundEffect->loops());
	PositionalSoundCreationInfo positionCreationalSoundInfo = PositionalSoundCreationInfo(
		transform->getTranslation());
	int soundIndex = m_audioBackendSystem->createPositionalSound(
		&creationalSoundInfo, &positionCreationalSoundInfo);
	positionalSoundEffect->setSoundIndex(soundIndex);
}

void PositionalSoundSystem::removed( Entity* p_entity )
{
	throw std::exception("The method or operation is not implemented.");
}

void PositionalSoundSystem::updateSoundPositions( const vector<Entity*>& p_entities )
{
	AudioInfo* audioInfo = NULL;
	Transform* transform = NULL;
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		audioInfo = static_cast<AudioInfo*>(p_entities[i]->getComponent(
			ComponentType::AudioInfo));
		transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));

		if(audioInfo->isPositionalSound())
		{
			PositionalSound* positionalSound = static_cast<PositionalSound*>(
				m_audioBackendSystem->getSoundWrapper()->getSound(
				audioInfo->getSoundId()));
			positionalSound->setPosition(transform->getTranslation());
		}
	}
}
