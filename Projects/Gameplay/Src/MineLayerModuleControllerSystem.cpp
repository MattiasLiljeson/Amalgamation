#include "MineLayerModuleControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include <Control.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "ShipModule.h"
#include "StandardMine.h"
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"
#include "SpawnSoundEffectPacket.h"

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
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (module->m_parentEntity >= 0)
		{
			MineLayerModule* mineLayer = static_cast<MineLayerModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::MineLayerModule)));

			mineLayer->m_cooldown -= dt;
			if (mineLayer->m_cooldown <= 0 && module->getActive())
			{
				mineLayer->m_cooldown = 2;
				Transform* transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(p_entities[i],
					ComponentType::getTypeFor(ComponentType::Transform)));
				PhysicsBody* physBody = static_cast<PhysicsBody*>(
					p_entities[i]->getComponent(ComponentType::PhysicsBody));
				PhysicsSystem* physics = static_cast<PhysicsSystem*>(
					m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
				AglVector3 moduleVelocity = physics->getController()->getBody(
					physBody->m_id)->GetVelocity();
				spawnMine(transform, moduleVelocity);
			}
		}
	}
}

void MineLayerModuleControllerSystem::spawnMine(Transform* p_transform,
												AglVector3 p_moduleVelocity)
{

	Entity* entity = m_world->createEntity();
	Transform* t = new Transform(p_transform->getTranslation(), p_transform->getRotation(),
		AglVector3(0.8f, 0.8f, 0.8f));
	entity->addComponent( ComponentType::Transform, t);
	entity->addComponent(ComponentType::StandardMine, new StandardMine());
	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );
	AglVector3 fireDirection = AglVector3(0, -1.0f, 0);
	const AglQuaternion& rot = p_transform->getRotation();
	rot.transformVector(fireDirection);
	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(),
		AglVector3(0.8f, 0.8f, 0.8f), fireDirection * 10.0f + p_moduleVelocity,
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