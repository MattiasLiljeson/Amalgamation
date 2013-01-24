#include <Sound.h>
#include <PositionalSound.h>
#include <SoundWrapper.h>

#include "PositionalSoundSystem.h"
#include "AudioBackendSystem.h"

#include "Transform.h"
#include "AudioInfo.h"
#include "PositionalSoundSource.h"
#include <Globals.h>
#include <PositionalSoundCreationInfo.h>

PositionalSoundSystem::PositionalSoundSystem()
	: EntitySystem( SystemType::PositionalSoundSystem, 2,
	ComponentType::PositionalSoundSource, ComponentType::Transform )
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
	PositionalSoundSource* positionalSoundSource = static_cast<PositionalSoundSource*>(
		p_entity->getComponent(ComponentType::PositionalSoundSource));
	Transform* transform = static_cast<Transform*>(
		p_entity->getComponent(ComponentType::Transform));

	BasicSoundCreationInfo creationalSoundInfo = BasicSoundCreationInfo(
		positionalSoundSource->getFilename().c_str(),
		positionalSoundSource->getPath().c_str(),
		positionalSoundSource->loops());
	PositionalSoundCreationInfo positionCreationalSoundInfo = PositionalSoundCreationInfo(
		transform->getTranslation());
	int soundIndex = m_audioBackendSystem->createPositionalSound(
		&creationalSoundInfo, &positionCreationalSoundInfo);
	positionalSoundSource->setSoundIndex(soundIndex);
	// NOTE: (Johan) There is a bug where STOP must be called before PLAY when playing
	// sound only once. If only PLAY is called the sound will play exactly two times.
	m_audioBackendSystem->getSoundWrapper()->updateSound(soundIndex, SoundEnums::STOP);
	m_audioBackendSystem->getSoundWrapper()->updateSound(soundIndex, SoundEnums::PLAY);
}

void PositionalSoundSystem::removed( Entity* p_entity )
{
	PositionalSoundSource* positionalSoundEffect = static_cast<PositionalSoundSource*>(
		p_entity->getComponent(ComponentType::PositionalSoundSource));
	int soundIndex = positionalSoundEffect->getSoundIndex();
	m_audioBackendSystem->getSoundWrapper()->updateSound(soundIndex, SoundEnums::STOP);
}

void PositionalSoundSystem::updateSoundPositions( const vector<Entity*>& p_entities )
{
	PositionalSoundSource* positionalSoundEffect = NULL;
	Transform* transform = NULL;
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		positionalSoundEffect = static_cast<PositionalSoundSource*>(p_entities[i]->getComponent(
			ComponentType::PositionalSoundSource));
		transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));

		PositionalSound* positionalSound = static_cast<PositionalSound*>(
			m_audioBackendSystem->getSoundWrapper()->getSound(
			positionalSoundEffect->getSoundIndex()));
		positionalSound->setPosition(transform->getTranslation());

		m_audioBackendSystem->updateOutputMatrix(positionalSoundEffect->getSoundIndex());
	}
}
