#include "SlotHighlightParticleMakerSystem.h"
#include "ConnectionPointSet.h"
#include "ParticleSystemsComponent.h"
#include "MeshOffsetTransform.h"

SlotHighlightParticleMakerSystem::SlotHighlightParticleMakerSystem()
	: EntitySystem(SystemType::SlotHighlightParticleMakerSystem, 3,
	ComponentType::TAG_MyShip, ComponentType::ConnectionPointSet,
	ComponentType::MeshOffsetTransform)
{
}

void SlotHighlightParticleMakerSystem::inserted( Entity* p_entity )
{
	ParticleSystemsComponent* emitters = static_cast<ParticleSystemsComponent*>(
		p_entity->getComponent( ComponentType::ParticleSystemsComponent ) );
	if( emitters == NULL )
	{
		emitters = new ParticleSystemsComponent();
		p_entity->addComponent( emitters );
	}
	ConnectionPointSet* connectionPoints = static_cast<ConnectionPointSet*>(p_entity->getComponent(
		ComponentType::ConnectionPointSet));
	if(connectionPoints->m_connectionPoints.size() >= 4)
	{
		AglVector3 positionBuffer; // The only usable.
		AglQuaternion rotationBuffer;
		AglVector3 scaleBuffer;
		AglMatrix inverseOffset = static_cast<MeshOffsetTransform*>(p_entity->
			getComponent(ComponentType::MeshOffsetTransform))->offset.inverse();
		AglMatrix transform;
		transform = connectionPoints->m_connectionPoints[0].cpTransform;
		transform *= inverseOffset;
		transform.toComponents(scaleBuffer, rotationBuffer, positionBuffer);
		createHighlightParticleEmitter(emitters, positionBuffer, positionBuffer, 0); // Down

		transform = connectionPoints->m_connectionPoints[1].cpTransform;
		transform *= inverseOffset;
		transform.toComponents(scaleBuffer, rotationBuffer, positionBuffer);
		createHighlightParticleEmitter(emitters, positionBuffer, positionBuffer, 1); // Forward

		transform = connectionPoints->m_connectionPoints[2].cpTransform;
		transform *= inverseOffset;
		transform.toComponents(scaleBuffer, rotationBuffer, positionBuffer);
		createHighlightParticleEmitter(emitters, positionBuffer, positionBuffer, 2); // Left

		transform = connectionPoints->m_connectionPoints[3].cpTransform;
		transform *= inverseOffset;
		transform.toComponents(scaleBuffer, rotationBuffer, positionBuffer);
		createHighlightParticleEmitter(emitters, positionBuffer, positionBuffer, 3); // Right
	}
}

void SlotHighlightParticleMakerSystem::createHighlightParticleEmitter( ParticleSystemsComponent* p_emitters,
	AglVector3 p_spawnPosition, AglVector3 p_spawnDirection, int p_desiredIndex )
{
	AglParticleSystem particleSystem;
	particleSystem.setSpawnPoint(p_spawnPosition);
	particleSystem.setSpawnDirection(p_spawnDirection);
	particleSystem.setParticleAge(1.0f);
	particleSystem.setSpawnSpeed(1.0f);
	particleSystem.setSpawnFrequency(1.0f);
	particleSystem.setParticlesPerSpawn(50);
	particleSystem.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem.setSpread(0.21f);
	particleSystem.setFadeOutStart(0.0f);
	particleSystem.setFadeInStop(0.0f);
	particleSystem.setSpawnType(AglParticleSystemHeader::ONCE);
	particleSystem.setAlignmentType(AglParticleSystemHeader::VELOCITY);
	particleSystem.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem.setParticleSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem.setRasterizerMode(AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP);
	ParticleSystemInstruction particleInstruction;
	particleInstruction.textureFileName = "red-spot.png";
	particleInstruction.particleSystem = particleSystem;
	p_emitters->addParticleSystemInstruction(particleInstruction, p_desiredIndex);
}