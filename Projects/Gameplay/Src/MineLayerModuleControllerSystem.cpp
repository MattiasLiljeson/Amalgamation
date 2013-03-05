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

MineLayerModuleControllerSystem::MineLayerModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MineLayerModuleControllerSystem, 2,
	ComponentType::MineLayerModule, ComponentType::PhysicsBody)
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
		MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entities[i]->getComponent(ComponentType::MeshOffsetTransform));

		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (module->m_parentEntity >= 0)
		{
			MineLayerModule* mineLayer = static_cast<MineLayerModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::MineLayerModule)));

			mineLayer->m_cooldown -= dt;
			if ((mineLayer->m_cooldown <= 0 && module->getActive()) || mineLayer->m_timeSinceMineSpawnStart > 0)
			{
				if (mineLayer->m_timeSinceMineSpawnStart > 1.0f)
				{
					Transform* transform = static_cast<Transform*>(
						m_world->getComponentManager()->getComponent(p_entities[i],
						ComponentType::getTypeFor(ComponentType::Transform)));
					PhysicsBody* physBody = static_cast<PhysicsBody*>(
						p_entities[i]->getComponent(ComponentType::PhysicsBody));
					PhysicsSystem* physics = static_cast<PhysicsSystem*>(
						m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
					AglVector3 moduleVelocity = physics->getController()->getBody(
						physBody->m_id)->GetVelocity();
					spawnMine(transform, moduleVelocity,module);
					mineLayer->m_timeSinceMineSpawnStart = 0;
				} 

				else if (mineLayer->m_timeSinceMineSpawnStart == 0)
				{
					setSpawnAnimation(p_entities[i]);
					mineLayer->m_timeSinceMineSpawnStart += dt;
				}
				else
					mineLayer->m_timeSinceMineSpawnStart += dt;
				mineLayer->m_cooldown = 2;
			}
		}
	}
}

void MineLayerModuleControllerSystem::spawnMine(Transform* p_transform,
												AglVector3 p_moduleVelocity,
												ShipModule* p_module)
{
	Entity* entity = m_world->createEntity();
	Transform* t = new Transform(p_transform->getTranslation(), p_transform->getRotation(),
		AglVector3(1.4f, 1.4f, 1.4f));
	entity->addComponent( ComponentType::Transform, t);



	// store owner data
	StandardMine* mineModule = new StandardMine();
	mineModule->m_ownerId = ModuleHelper::FindParentShipClientId(m_world, p_module);
	entity->addComponent(ComponentType::StandardMine, mineModule);


	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );
	AglVector3 fireDirection = AglVector3(0, -1.0f, 0);
	const AglQuaternion& rot = p_transform->getRotation();
	rot.transformVector(fireDirection);
	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(),
		AglVector3(1.4f, 1.4f, 1.4f), fireDirection * 10.0f + p_moduleVelocity,
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, true));
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