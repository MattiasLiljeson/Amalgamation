#include "AnomalyAcceleratorModuleControllerSystem.h"
#include "ShipModule.h"
#include "AnomalyAcceleratorModule.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "BodyInitData.h"
#include "NetworkSynced.h"
#include "LoadMesh.h"
#include "EntityCreationPacket.h"
#include <TcpServer.h>
#include "ModuleHelper.h"
#include "AnomalyBomb.h"
#include "EntityFactory.h"
#include "MeshOffsetTransform.h"
#include "AnimationUpdatePacket.h"
#include "SpawnPointSet.h"

AnomalyAcceleratorModuleControllerSystem::AnomalyAcceleratorModuleControllerSystem(
	TcpServer* p_server)
	: EntitySystem(SystemType::AnomalyAcceleratorModuleControllerSystem, 5,
	ComponentType::AnomalyAcceleratorModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::ShipModule,
	ComponentType::MeshOffsetTransform)
{
	m_server = p_server;
}

void AnomalyAcceleratorModuleControllerSystem::processEntities(
	const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			p_entities[i]->getComponent(ComponentType::ShipModule));

		AnomalyAcceleratorModule* anomalyAccelerator = static_cast<
			AnomalyAcceleratorModule*>(p_entities[i]->getComponent(
			ComponentType::AnomalyAcceleratorModule));

		if(module->isOwned())
		{
			anomalyAccelerator->cooldown -= dt;
			if (anomalyAccelerator->cooldown <= 0)
			{
				if (anomalyAccelerator->currentBomb < 0)
				{
					spawnAnomalyBomb(p_entities[i]);
					playOpenAnimation(p_entities[i]);
				}
				else
				{
					updateBomb(p_entities[i], min(-anomalyAccelerator->cooldown, 1.0f));
				}
			}
			anomalyAccelerator->owned = true;

			if(anomalyAccelerator->cooldown <= 0.0f && (module->getActive() || anomalyAccelerator->timeSinceLaunchStart > 0))
			{
				if (anomalyAccelerator->timeSinceLaunchStart >= 0.5f)
				{
					anomalyAccelerator->cooldown = anomalyAccelerator->cooldownTime;
					anomalyAccelerator->timeSinceLaunchStart = 0;
					launchBomb(p_entities[i]);
				}
				else
				{
					//Play the release animation
					if (anomalyAccelerator->timeSinceLaunchStart == 0)
						playReleaseAnimation(p_entities[i]);

					anomalyAccelerator->timeSinceLaunchStart += dt;
				}
			}
		}
		else
		{
			anomalyAccelerator->owned = false;
		}
	}
}

void AnomalyAcceleratorModuleControllerSystem::spawnAnomalyBomb(Entity* p_entity )
{
	AnomalyAcceleratorModule* anomalyAccelerator = static_cast<
		AnomalyAcceleratorModule*>(p_entity->getComponent(
		ComponentType::AnomalyAcceleratorModule));

	ShipModule* module = static_cast<ShipModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ShipModule)));

	Transform* layerTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	PhysicsSystem* physics = static_cast<PhysicsSystem*>
		( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

	PhysicsBody* body = static_cast<PhysicsBody*>
		( p_entity->getComponent( ComponentType::PhysicsBody) );

	Body* rigidBody = physics->getController()->getBody(body->m_id);

	MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
		( p_entity->getComponent( ComponentType::MeshOffsetTransform) );


	//Find spawn point
	SpawnPointSet* sps = static_cast<SpawnPointSet*>(p_entity->getComponent(ComponentType::SpawnPointSet));
	AglMatrix bombOffset = AglMatrix::identityMatrix();
	for (unsigned int sp = 0; sps->m_spawnPoints.size(); sp++)
	{
		if (sps->m_spawnPoints[sp].spAction == "Bomb")
		{
			bombOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	//Fix!
	Transform* t = new Transform(bombOffset*meshOffset->offset.inverse()*body->getOffset().inverse()*rigidBody->GetWorld());



	Entity* entity = m_world->createEntity();
	entity->addComponent( ComponentType::Transform, t);

	entity->addComponent(ComponentType::NetworkSynced, 
		new NetworkSynced( entity->getIndex(), -1, EntityType::AnomalyBomb));


	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::AnomalyBomb);
	data.owner			= -1;
	data.networkIdentity = entity->getIndex();
	data.translation	= t->getTranslation();
	data.rotation		= t->getRotation();
	data.scale			= t->getScale();
	data.meshInfo		= 1;
	m_server->broadcastPacket(data.pack());

	anomalyAccelerator->currentBomb = entity->getIndex();

	entity->addComponent(ComponentType::LoadMesh, new LoadMesh("anomaly_bomb.agl"));

	m_world->addEntity(entity);




	/*Entity* bombEntity = static_cast<EntityFactory*>(m_world->getSystem(
		SystemType::EntityFactory))->createAnomalyBombServer(p_transform,
		p_moduleVelocity, p_module);
	EntityCreationPacket data;
	Transform* bombTransform = static_cast<Transform*>(bombEntity->getComponent(
		ComponentType::Transform));
	if(bombTransform != NULL)
	{
		data.translation	= bombTransform->getTranslation();
		data.rotation		= bombTransform->getRotation();
		data.scale			= bombTransform->getScale();
	}
	data.entityType		= static_cast<char>(EntityType::AnomalyBomb);
	data.owner			= -1;
	data.networkIdentity = bombEntity->getIndex();
	data.meshInfo		= 1;
	m_server->broadcastPacket(data.pack());*/
}
void AnomalyAcceleratorModuleControllerSystem::updateBomb(Entity* p_entity, float p_age)
{
	AnomalyAcceleratorModule* anomalyAccelerator = static_cast<
		AnomalyAcceleratorModule*>(p_entity->getComponent(
		ComponentType::AnomalyAcceleratorModule));

	ShipModule* module = static_cast<ShipModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ShipModule)));

	Transform* layerTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	PhysicsSystem* physics = static_cast<PhysicsSystem*>
		( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

	PhysicsBody* body = static_cast<PhysicsBody*>
		( p_entity->getComponent( ComponentType::PhysicsBody) );

	Body* rigidBody = physics->getController()->getBody(body->m_id);

	MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
		( p_entity->getComponent( ComponentType::MeshOffsetTransform) );

	Entity* bomb = m_world->getEntity(anomalyAccelerator->currentBomb);

	MeshOffsetTransform* bombMeshOffset = static_cast<MeshOffsetTransform*>
		( bomb->getComponent( ComponentType::MeshOffsetTransform) );

	//Find spawn point
	SpawnPointSet* sps = static_cast<SpawnPointSet*>(p_entity->getComponent(ComponentType::SpawnPointSet));
	AglMatrix bombOffset = AglMatrix::identityMatrix();
	for (unsigned int sp = 0; sps->m_spawnPoints.size(); sp++)
	{
		if (sps->m_spawnPoints[sp].spAction == "Bomb")
		{
			bombOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	float size = 0.1f + 0.9f * p_age;
	AglMatrix toSet = AglMatrix::createScaleMatrix(AglVector3(size, size, size)) * bombMeshOffset->offset
		*bombOffset*meshOffset->offset.inverse()*body->getOffset().inverse()*rigidBody->GetWorld();

	Transform* bombTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(bomb,
		ComponentType::getTypeFor(ComponentType::Transform)));
	bombTransform->setMatrix(toSet);
}
void AnomalyAcceleratorModuleControllerSystem::launchBomb(Entity* p_entity)
{
	PhysicsSystem* physics = static_cast<PhysicsSystem*>
		( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

	PhysicsBody* body = static_cast<PhysicsBody*>
		( p_entity->getComponent( ComponentType::PhysicsBody) );

	Body* rigidBody = physics->getController()->getBody(body->m_id);

	AnomalyAcceleratorModule* layer = static_cast<AnomalyAcceleratorModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::AnomalyAcceleratorModule)));

	Entity* bomb = m_world->getEntity(layer->currentBomb);

	Transform* transform = static_cast<Transform*>(bomb->getComponent(ComponentType::Transform));
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(bomb->getComponent(ComponentType::MeshOffsetTransform));

	AglVector3 dir = (offset->offset*transform->getMatrix()).GetUp();

	bomb->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	bomb->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(transform->getTranslation(),
		transform->getRotation(),
		AglVector3(1.0f, 1.0f, 1.0f), rigidBody->GetVelocity()+dir*50,
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, false));

	AnomalyBomb* abomb = new AnomalyBomb(17, 15, 125, 10, 2, 120);
	bomb->addComponent(ComponentType::AnomalyBomb, abomb);

	bomb->applyComponentChanges();

	layer->currentBomb = -1;
}
void AnomalyAcceleratorModuleControllerSystem::playReleaseAnimation(Entity* p_accelerator)
{
	AnimationUpdatePacket packet;
	packet.networkIdentity = p_accelerator->getIndex();
	packet.shouldPlay = true;
	packet.playSpeed = 15.0f;
	packet.take = "Dropping";
	m_server->broadcastPacket( packet.pack() );

	packet.shouldQueue = true;
	packet.take = "Idle";
	m_server->broadcastPacket( packet.pack() );
}
void AnomalyAcceleratorModuleControllerSystem::playOpenAnimation(Entity* p_accelerator)
{
	AnimationUpdatePacket packet;
	packet.networkIdentity = p_accelerator->getIndex();
	packet.shouldPlay = true;
	packet.shouldQueue = false;
	packet.playSpeed = 15.0f;
	packet.take = "Opening";
	m_server->broadcastPacket( packet.pack() );

	packet.take = "Open";
	packet.shouldQueue = true;
	m_server->broadcastPacket( packet.pack() );
}