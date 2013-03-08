#include "MineLayerModuleControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "ShipModule.h"
#include "StandardMine.h"
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"
#include "SpawnSoundEffectPacket.h"
#include "ModuleHelper.h"
#include "AnimationUpdatePacket.h"
#include "MeshOffsetTransform.h"
#include "SpawnPointSet.h"
#include "ShipConnectionPointHighlights.h"

MineLayerModuleControllerSystem::MineLayerModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MineLayerModuleControllerSystem, 3,
	ComponentType::MineLayerModule, ComponentType::PhysicsBody,
	ComponentType::MeshOffsetTransform)
{
	m_server = p_server;
}


MineLayerModuleControllerSystem::~MineLayerModuleControllerSystem()
{
}

void MineLayerModuleControllerSystem::initialize()
{
}

void MineLayerModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (module->m_parentEntity >= 0)
		{
			//Check if the layer is highlighted
			Entity* child = p_entities[i];
			Entity* parent = NULL;

			bool highlighted = false;
			while (true)
			{
				parent = m_world->getEntity(module->m_parentEntity);
				ShipModule* parentmodule = static_cast<ShipModule*>(
					m_world->getComponentManager()->getComponent(parent,
					ComponentType::getTypeFor(ComponentType::ShipModule)));
				if (!parentmodule)
					break;
				else
				{
					module = parentmodule;
					child = parent;
				}
			}

			ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(parent->getComponent(ComponentType::ConnectionPointSet));
			ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
				parent->getComponent(ComponentType::ShipConnectionPointHighlights) );

			for (unsigned int j=0;j<ShipConnectionPointHighlights::slots;j++)
			{
				if (highlights->slotStatus[j])
				{
					if (cps->m_connectionPoints[j].cpConnectedEntity == child->getIndex())
					{
						highlighted = true;
						break;
					}
				}
			}




			MineLayerModule* mineLayer = static_cast<MineLayerModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::MineLayerModule)));

			mineLayer->m_mineAge += dt*highlighted;

			if (mineLayer->m_currentMine < 0 && highlighted)
			{
				spawnMine(p_entities[i]);
				mineLayer->m_mineAge = 0;
			}
			else if (mineLayer->m_currentMine >= 0)
				updateMine(p_entities[i], min(mineLayer->m_mineAge / 2.5f, 1.0f));

			if (highlighted)
			{
				if (mineLayer->m_mineAge >= 2.5f && (module->getActive() || mineLayer->m_timeSinceSpawnStarted > 0))
				{
					if (mineLayer->m_timeSinceSpawnStarted == 0)
					{
						setSpawnAnimation(p_entities[i]);
					}

					mineLayer->m_timeSinceSpawnStarted += dt;

					if (mineLayer->m_timeSinceSpawnStarted >= 0.5f)
					{
						launchMine(p_entities[i]);
						mineLayer->m_currentMine = -1;
						mineLayer->m_timeSinceSpawnStarted = 0.0f;
					}
				}
			}
		}
	}
}

void MineLayerModuleControllerSystem::spawnMine(Entity* p_entity)
{
	MineLayerModule* layer = static_cast<MineLayerModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MineLayerModule)));

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
	AglMatrix mineOffset = AglMatrix::identityMatrix();
	for (unsigned int sp = 0; sps->m_spawnPoints.size(); sp++)
	{
		if (sps->m_spawnPoints[sp].spAction == "Mine")
		{
			mineOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	//Fix!
	Transform* t = new Transform(mineOffset*meshOffset->offset.inverse()*body->getOffset().inverse()*rigidBody->GetWorld());



	Entity* entity = m_world->createEntity();
	entity->addComponent( ComponentType::Transform, t);



	// store owner data
	StandardMine* mineModule = new StandardMine();
	mineModule->m_ownerId = ModuleHelper::FindParentShipClientId(m_world, module);
	entity->addComponent(ComponentType::StandardMine, mineModule);


	/*entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );
	AglVector3 fireDirection = AglVector3(0, 0, 1.0f);
	const AglQuaternion& rot = p_transform->getRotation();
	rot.transformVector(fireDirection);
	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(),
		AglVector3(1.0f, 1.0f, 1.0f), fireDirection * 75.0f + p_moduleVelocity,
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, true));*/
	entity->addComponent(ComponentType::NetworkSynced, 
		new NetworkSynced( entity->getIndex(), -1, EntityType::Mine));


	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::Mine);
	data.owner			= -1;
	data.networkIdentity = entity->getIndex();
	data.translation	= t->getTranslation();
	data.rotation		= t->getRotation();
	data.scale			= t->getScale();
	data.meshInfo		= 1;
	m_server->broadcastPacket(data.pack());

	SpawnSoundEffectPacket soundEffectPacket;
	soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::MineUnload;
	soundEffectPacket.positional = true;
	soundEffectPacket.position = t->getTranslation();
	soundEffectPacket.attachedToNetsyncEntity = -1;
	m_server->broadcastPacket(soundEffectPacket.pack());

	m_world->addEntity(entity);
	layer->m_currentMine = entity->getIndex();
}
void MineLayerModuleControllerSystem::updateMine(Entity* p_entity, float p_age)
{
	MineLayerModule* layer = static_cast<MineLayerModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MineLayerModule)));

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
	AglMatrix mineOffset = AglMatrix::identityMatrix();
	for (unsigned int sp = 0; sps->m_spawnPoints.size(); sp++)
	{
		if (sps->m_spawnPoints[sp].spAction == "Mine")
		{
			mineOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	Entity* mine = m_world->getEntity(layer->m_currentMine);
	Transform* mineTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(mine,
		ComponentType::getTypeFor(ComponentType::Transform)));
	mineTransform->setMatrix(AglMatrix::createScaleMatrix(AglVector3(p_age, p_age, p_age))
		*mineOffset*meshOffset->offset.inverse()*body->getOffset().inverse()*rigidBody->GetWorld());
}
void MineLayerModuleControllerSystem::launchMine(Entity* p_entity)
{
	PhysicsSystem* physics = static_cast<PhysicsSystem*>
		( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

	PhysicsBody* body = static_cast<PhysicsBody*>
		( p_entity->getComponent( ComponentType::PhysicsBody) );

	Body* rigidBody = physics->getController()->getBody(body->m_id);

	MineLayerModule* layer = static_cast<MineLayerModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MineLayerModule)));

	Entity* mine = m_world->getEntity(layer->m_currentMine);

	Transform* transform = static_cast<Transform*>(mine->getComponent(ComponentType::Transform));

	mine->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	mine->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(transform->getTranslation(),
		transform->getRotation(),
		AglVector3(1.4f, 1.4f, 1.4f), -transform->getForward()*20 + rigidBody->GetVelocity(),
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, true));

	mine->applyComponentChanges();
}
void MineLayerModuleControllerSystem::setSpawnAnimation(Entity* p_layer)
{
	AnimationUpdatePacket packet;
	packet.networkIdentity = p_layer->getIndex();
	packet.shouldPlay = true;
	packet.playSpeed = 1.0f;
	packet.take = "Spawn";
	m_server->broadcastPacket( packet.pack() );

	AnimationUpdatePacket packetIdle;
	packetIdle.networkIdentity = p_layer->getIndex();
	packetIdle.shouldPlay = true;
	packetIdle.playSpeed = 15.0f;
	packetIdle.take = "Default";
	packetIdle.shouldQueue = true;
	m_server->broadcastPacket( packetIdle.pack() );
}