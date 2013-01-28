#include "ShieldModuleControllerSystem.h"
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
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"

ShieldModuleControllerSystem::ShieldModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::ShieldModuleControllerSystem, 1, ComponentType::ShieldModule)
{
	m_server = p_server;
}


ShieldModuleControllerSystem::~ShieldModuleControllerSystem()
{
}

void ShieldModuleControllerSystem::initialize()
{
}

void ShieldModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));
		if (module && module->m_parentEntity >= 0)
		{
			ShieldModule* shieldModule = static_cast<ShieldModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::getTypeFor(ComponentType::ShieldModule)));
			
			handleShieldEntity(shieldModule, m_world->getEntity(module->m_parentEntity), module->m_active);
		}
	}
}
void ShieldModuleControllerSystem::handleShieldEntity(ShieldModule* p_module, Entity* p_parentEntity, bool p_active)
{
	Transform* parentTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_parentEntity,
		ComponentType::getTypeFor(ComponentType::Transform))); 

	if (p_module->m_shieldEntity >= 0)
	{
		//Update shield
		Entity* shield = m_world->getEntity(p_module->m_shieldEntity);
		
		Transform* transform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(shield,
			ComponentType::getTypeFor(ComponentType::Transform))); 
		
		if (p_active)
		{
			transform->setTranslation(parentTransform->getTranslation());
			transform->setRotation(parentTransform->getRotation());
			transform->setScale(AglVector3(6, 6, 6));
		}
		else
		{
			transform->setScale(AglVector3(0, 0, 0));
		}
	}
	else
	{
		//Create Shield
		Entity* entity = m_world->createEntity();

		Transform* t = new Transform(parentTransform->getTranslation(), AglQuaternion::identity(), AglVector3(2, 2, 2));
		entity->addComponent( ComponentType::Transform, t);
		
		EntityCreationPacket data;
		data.entityType		= static_cast<char>(EntityType::Shield);
		data.meshInfo		= 1; //Sphere
		data.owner			= -1;
		data.networkIdentity = entity->getIndex();
		data.translation	= t->getTranslation();
		data.rotation		= t->getRotation();
		data.scale			= t->getScale();

		entity->addComponent(ComponentType::NetworkSynced, 
			new NetworkSynced( entity->getIndex(), -1, EntityType::Shield));

		m_server->broadcastPacket(data.pack());
		
		m_world->addEntity(entity);

		p_module->m_shieldEntity = entity->getIndex();
	}
}