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

AnomalyAcceleratorModuleControllerSystem::AnomalyAcceleratorModuleControllerSystem(
	TcpServer* p_server)
	: EntitySystem(SystemType::AnomalyAcceleratorModuleControllerSystem, 4,
	ComponentType::AnomalyAcceleratorModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::ShipModule)
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
		if(module->isOwned())
		{
			AnomalyAcceleratorModule* anomalyAccelerator = static_cast<
				AnomalyAcceleratorModule*>(p_entities[i]->getComponent(
				ComponentType::AnomalyAcceleratorModule));
			anomalyAccelerator->cooldown -= dt;
			if(anomalyAccelerator->cooldown <= 0.0f && module->getActive())
			{
				anomalyAccelerator->cooldown = 1.0f;
				Transform* transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(p_entities[i],
					ComponentType::getTypeFor(ComponentType::Transform)));
				PhysicsBody* physBody = static_cast<PhysicsBody*>(
					p_entities[i]->getComponent(ComponentType::PhysicsBody));
				PhysicsSystem* physics = static_cast<PhysicsSystem*>(
					m_world->getSystem(SystemType::PhysicsSystem));
				AglVector3 moduleVelocity = physics->getController()->getBody(
					physBody->m_id)->GetVelocity();
				spawnAnomalyBomb(transform, moduleVelocity, module);
			}
		}
	}
}

void AnomalyAcceleratorModuleControllerSystem::spawnAnomalyBomb( Transform* p_transform,
	AglVector3 p_moduleVelocity, ShipModule* p_module )
{
	Entity* bombEntity = m_world->createEntity();
	bombEntity->setName("AnomalyBomb");
	AglVector3 scale = AglVector3(1.0f, 1.0f, 1.0f);
	Transform* bombTransform = new Transform(p_transform->getTranslation(),
		p_transform->getRotation(), scale);
	bombEntity->addComponent(bombTransform);
	bombEntity->addComponent(new PhysicsBody());
	bombEntity->addComponent(new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(), scale, p_moduleVelocity,
		AglVector3::zero(), 0, BodyInitData::DYNAMIC, BodyInitData::SINGLE,
		false, true));
	bombEntity->addComponent(new AnomalyBomb());
	bombEntity->addComponent(new NetworkSynced(bombEntity->getIndex(), -1,
		EntityType::AnomalyBomb));
	m_world->addEntity(bombEntity);

	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::AnomalyBomb);
	data.owner			= -1;
	data.networkIdentity = bombEntity->getIndex();
	data.translation	= bombTransform->getTranslation();
	data.rotation		= bombTransform->getRotation();
	data.scale			= bombTransform->getScale();
	data.meshInfo		= 1;
	m_server->broadcastPacket(data.pack());
}