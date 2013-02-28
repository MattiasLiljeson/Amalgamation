#include "ShieldPlaterSystem.h"
#include "Transform.h"
#include "LoadMesh.h"
#include "EntityParent.h"
#include "ShieldPlate.h"
#include "ShipModule.h"
#include "ShieldModuleActivationClient.h"
#include "AudioBackendSystem.h"
#include "SpawnPointSet.h"

ShieldPlaterSystem::ShieldPlaterSystem()
	: EntitySystem(SystemType::ShieldPlaterSystem, 4, ComponentType::ShieldModule,
	ComponentType::Transform, ComponentType::SpawnPointSet, ComponentType::ShipModule)
{
}

void ShieldPlaterSystem::inserted( Entity* p_entity )
{
	const int plateCount = 120;
	vector<Entity*> plateEntities;
	plateEntities.resize(120);
	for(unsigned int i=0; i<plateCount; i++)
	{
		Entity* entity = m_world->createEntity();
		entity->setName("shieldPlateClient");
		plateEntities[i] = entity;
		float spawnX, spawnY;
		circularRandom(&spawnX, &spawnY, true);
		SpawnPointSet* spawnPointSet = static_cast<SpawnPointSet*>(p_entity->
			getComponent(ComponentType::SpawnPointSet));
		AglVector3 spawnPoint;
		AglVector3 scaleBuffer;
		AglQuaternion quaternionBuffer;
		spawnPointSet->m_spawnPoints[0].spTransform.toComponents(scaleBuffer,
			quaternionBuffer, spawnPoint);
		float radius = 10.0f;
		AglVector3 position = spawnPoint + AglVector3(radius * spawnX, radius * spawnY, 0 );
		position.normalize();
		AglQuaternion plateRotation = AglQuaternion::rotateToFrom(AglVector3(0, 1.0f, 0.0f), position);
		position = position * spawnPoint.length();
		float plateScale = 1.0f;
		Transform* plateTransform = new Transform(position, plateRotation,
			AglVector3(plateScale, plateScale, plateScale));
		entity->addComponent(plateTransform);
		entity->addComponent(new LoadMesh("shield_plate.agl"));
		entity->addComponent(new EntityParent(p_entity->getIndex(),
			plateTransform->getMatrix()));
		entity->addComponent(new ShieldPlate(0.2f + 0.8f * (float)rand()/(float)RAND_MAX));
		entity->setEnabled(false);
		m_world->addEntity(entity);
	}
	ShipModule* shipModule = static_cast<ShipModule*>(p_entity->getComponent(
		ComponentType::ShipModule));
	shipModule->addActivationEvent(new ShieldModuleActivationClient(plateEntities,
		p_entity, static_cast<AudioBackendSystem*>(
		m_world->getSystem(SystemType::AudioBackendSystem))));
}

void ShieldPlaterSystem::circularRandom( float* p_spawnX, float* p_spawnY,
	bool p_warpCompensation )
{
	if(!p_warpCompensation)
	{
		float x = 0.0f;
		float y = 0.0f;
		do
		{
			x = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			y = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
		} while( x * x + y * y > 1.0f );
		*p_spawnX = x;
		*p_spawnY = y;
	}
	else
	{
		// NOTE: This version of circular random compensates for sphere warping
		// quite roughly. With not too high of a warp angle it looks nice.
		// It is much slower than the simple circular random generation.
		float x = 0.0f;
		float y = 0.0f;
		do
		{
			float x_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			float y_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			x = x_rand;
			y = y_rand;
		} while( x * x + y * y > 1.0f );
		float len = sqrt(x * x + y * y);
		x = x * len;
		y = y * len;
		*p_spawnX = x;
		*p_spawnY = y;
	}
}