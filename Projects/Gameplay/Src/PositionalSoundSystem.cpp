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
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		updateSoundPositions(p_entities[i]);
	}
}

void PositionalSoundSystem::initialize()
{
	m_audioBackendSystem = static_cast<AudioBackendSystem*>(m_world->getSystem(
		SystemType::AudioBackendSystem ));
}

void PositionalSoundSystem::inserted( Entity* p_entity )
{
	SoundComponent* positionalSoundSource = static_cast<SoundComponent*>(
		p_entity->getComponent(ComponentType::SoundComponent));
	Transform* transform = static_cast<Transform*>(
		p_entity->getComponent(ComponentType::Transform));
	/*
	BasicSoundCreationInfo creationalSoundInfo = BasicSoundCreationInfo(
		positionalSoundSource->getFilename().c_str(),
		positionalSoundSource->getPath().c_str(),
		positionalSoundSource->loops());
	PositionalSoundCreationInfo positionCreationalSoundInfo = PositionalSoundCreationInfo(
		transform->getTranslation());
	int soundIndex = m_audioBackendSystem->getSoundWrapper()->createNewPositionalSound(
		&creationalSoundInfo, &positionCreationalSoundInfo);
	positionalSoundSource->setSoundIndex(soundIndex);
	updateSoundPositions(p_entity);
	m_audioBackendSystem->getSoundWrapper()->updateSound(soundIndex, SoundEnums::PLAY);
	*/
}

void PositionalSoundSystem::removed( Entity* p_entity )
{
	/*
	SoundComponent* positionalSoundEffect = static_cast<SoundComponent*>(
		p_entity->getComponent(ComponentType::SoundComponent));
	int soundIndex = positionalSoundEffect->getSoundIndex();
	m_audioBackendSystem->getSoundWrapper()->updateSound(soundIndex, SoundEnums::STOP);
	*/
}

void PositionalSoundSystem::updateSoundPositions( Entity* p_entity )
{
	/*
	SoundComponent* positionalSoundEffect = NULL;
	Transform* transform = NULL;
	positionalSoundEffect = static_cast<SoundComponent*>(p_entity->getComponent(
		ComponentType::SoundComponent));
	transform = static_cast<Transform*>(p_entity->getComponent(
		ComponentType::Transform));

	PositionalSound* positionalSound = static_cast<PositionalSound*>(
		m_audioBackendSystem->getSoundWrapper()->getSound(
		positionalSoundEffect->getSoundIndex()));

	positionalSound->setPosition(transform->getTranslation());
	positionalSound->setFront(positionalSoundEffect->m_front);
	//positionalSound->setTop(positionalSoundEffect->m_top);

	m_audioBackendSystem->updateOutputMatrix(positionalSoundEffect->getSoundIndex());
	*/
}