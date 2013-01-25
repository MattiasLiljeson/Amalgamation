#include "MineLayerModuleControllerSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include <Control.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "StandardMine.h"
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"

MineLayerModuleControllerSystem::MineLayerModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MineLayerModuleControllerSystem, 1, ComponentType::MineLayerModule)
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
			if (mineLayer->m_cooldown <= 0 && module->m_active)
			{
				mineLayer->m_cooldown = 2;
				Transform* transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(p_entities[i],
					ComponentType::getTypeFor(ComponentType::Transform)));
				spawnMine(transform);
			}
		}
	}
}

void MineLayerModuleControllerSystem::spawnMine(Transform* p_transform)
{
	Entity* entity = m_world->createEntity();

	Transform* t = new Transform(p_transform->getTranslation(), p_transform->getRotation(), AglVector3(0.8f, 0.8f, 0.8f));
	entity->addComponent( ComponentType::Transform, t);

	entity->addComponent(ComponentType::StandardMine, new StandardMine());

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(),
		AglVector3(0.8f, 0.8f, 0.8f), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, true));


	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::Mine);
	data.owner			= -1;
	data.networkIdentity = entity->getIndex();
	data.translation	= t->getTranslation();
	data.rotation		= t->getRotation();
	data.scale			= t->getScale();
	data.meshInfo		= 1;

	entity->addComponent(ComponentType::NetworkSynced, 
		new NetworkSynced( entity->getIndex(), -1, EntityType::Mine));

	m_server->broadcastPacket(data.pack());

	m_world->addEntity(entity);
}