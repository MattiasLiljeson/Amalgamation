#include "EditSphereSystem.h"

#include <AglQuaternion.h>
#include "Transform.h"
#include "ShipInputProcessingSystem.h"
#include "GameplayTags.h"
#include "ShipSlotControllerSystem.h"
#include "ParticleSystemsComponent.h"
#include "..\..\AgileAssets\src\AglParticleSystem.h"
#include "LookAtEntity.h"

EditSphereSystem::EditSphereSystem() : 
EntitySystem( SystemType::EditSphereSystem, 1,
			 ComponentType::ComponentTypeIdx::TAG_Ship)

{
}

EditSphereSystem::~EditSphereSystem()
{

}

void EditSphereSystem::initialize()
{
	/************************************************************************/
	/* Attach a particle system for visualizing edit mode					*/
	/************************************************************************/
	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	//Flares spreading
	AglParticleSystem flares;
	flares.setSpawnPoint(AglVector3(0, 0, 0));
	flares.setSpawnDirection(AglVector3(0, 1, 0));
	flares.setSpread(1.0f);
	flares.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	flares.setSpawnOffset(10);
	flares.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	flares.setParticlesPerSpawn(10);
	flares.setSpawnFrequency(20.0f);
	flares.setAlignmentType(AglParticleSystemHeader::SCREEN);
	flares.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	flares.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	flares.setParticleSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	flares.setSpawnSpeed(0.0f);
	flares.setParticleSize(AglVector2(1.0f, 1.0f));
	flares.setParticleAge(1.0f);
	flares.setFadeOutStart(0.0f);
	flares.setMaxOpacity(0.5f);

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = "yellow-spot.png";
	particleInstructionFlares.particleSystem = flares;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	m_editSphere = m_world->createEntity();
	m_editSphere->addComponent( ComponentType::Transform, new Transform());
	m_editSphere->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(m_editSphere);

}

void EditSphereSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0)
	{
		float dt = m_world->getDelta();

		Transform* shipTransform = static_cast<Transform*>(p_entities[0]->getComponent(ComponentType::Transform));

		Transform* particleTransform = static_cast<Transform*>(m_editSphere->getComponent(ComponentType::Transform));
		
		particleTransform->setMatrix(shipTransform->getMatrix());

		ParticleSystemsComponent* particleEmitter = static_cast<ParticleSystemsComponent*>(m_editSphere->getComponent(ComponentType::ParticleSystemsComponent));
		if (p_entities[0]->getComponent(ComponentType::TAG_ShipEditMode))
		{
			particleEmitter->getParticleSystemPtr(0)->setSpawnFrequency(20.0f);
		}
		else
		{
			particleEmitter->getParticleSystemPtr(0)->setSpawnFrequency(0);
		}
	}
}
void EditSphereSystem::setSphere(AglVector3 p_position, float p_radius)
{
	ParticleSystemsComponent* particleEmitter = static_cast<ParticleSystemsComponent*>(m_editSphere->getComponent(ComponentType::ParticleSystemsComponent));
	particleEmitter->getParticleSystemPtr(0)->setSpawnOffset(p_radius);
	particleEmitter->getParticleSystemPtr(0)->setSpawnPoint(p_position);
}
