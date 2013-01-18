#include "RocketLauncherModuleControllerSystem.h"
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
#include "RocketLauncherModule.h"
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"
#include <PhysicsController.h>
#include "StandardRocket.h"

RocketLauncherModuleControllerSystem::RocketLauncherModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::RocketLauncherModuleControllerSystem, 1, ComponentType::RocketLauncherModule)
{
	m_server = p_server;
}


RocketLauncherModuleControllerSystem::~RocketLauncherModuleControllerSystem()
{
}

void RocketLauncherModuleControllerSystem::initialize()
{
}

void RocketLauncherModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (gun && module && module->m_parentEntity >= 0)
		{
			handleLaserSight(p_entities[i]);

			//Check fire
			gun->coolDown = max(0, gun->coolDown - dt);

			if (gun->coolDown == 0 && module->m_active)
			{
				spawnRocket(p_entities[i]);
			}
		}
	}
}

void RocketLauncherModuleControllerSystem::handleLaserSight(Entity* p_entity)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	if (gun->laserSightEntity < 0)
	{
		Entity* entity = m_world->createEntity();

		Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection), AglVector3(0.03f, 0.03f, 20));
		entity->addComponent( ComponentType::Transform, t);
		m_world->addEntity(entity);
		gun->laserSightEntity = entity->getIndex();

		EntityCreationPacket data;
		data.entityType		= static_cast<char>(EntityType::ShipModule);
		data.owner			= -1;
		data.networkIdentity = entity->getIndex();
		data.translation	= t->getTranslation();
		data.rotation		= t->getRotation();
		data.scale			= t->getScale();

		entity->addComponent(ComponentType::NetworkSynced, 
			new NetworkSynced( entity->getIndex(), -1, EntityType::ShipModule));

		m_server->broadcastPacket(data.pack());
	}
	else
	{
		Transform* gunTransform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(p_entity,
			ComponentType::getTypeFor(ComponentType::Transform)));

		Entity* entity = m_world->getEntity(gun->laserSightEntity);

		Transform* laserTransform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(entity,
			ComponentType::getTypeFor(ComponentType::Transform)));

		AglQuaternion rot = gunTransform->getRotation()*AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection);

		AglVector3 offset = AglVector3(0.03f, 0.03f, 20.0f);
		rot.transformVector(offset);
		laserTransform->setTranslation(gunTransform->getTranslation()+offset);
		laserTransform->setRotation(rot);
		//laserTransform->setTranslation(scale);
	}
}
void RocketLauncherModuleControllerSystem::spawnRocket(Entity* p_entity)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	gun->coolDown = 1.0f;

	//Create Rocket
	Transform* gunTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	Transform* t = new Transform(gunTransform->getTranslation(), AglQuaternion::identity(), AglVector3(0.8f, 0.8f, 0.8f));

	AglVector3 dir = gun->fireDirection;
	const AglQuaternion& rot = gunTransform->getRotation();
	rot.transformVector(dir);

	PhysicsBody* body = static_cast<PhysicsBody*>(p_entity->getComponent(ComponentType::PhysicsBody));

	PhysicsSystem* physics = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
	AglVector3 vel = physics->getController()->getBody(body->m_id)->GetVelocity();


	Entity* entity = m_world->createEntity();

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(gunTransform->getTranslation(),
		AglQuaternion::identity(),
		AglVector3(0.8f, 0.8f, 0.8f), dir * 100.0f + vel, 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, false));

	entity->addComponent( ComponentType::Transform, t);

	entity->addComponent(ComponentType::StandardRocket, new StandardRocket());
	m_world->addEntity(entity);

	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::ShipModule);
	data.owner			= -1;
	data.networkIdentity = entity->getIndex();
	data.translation	= t->getTranslation();
	data.rotation		= t->getRotation();
	data.scale			= t->getScale();
	data.meshInfo		= 1;

	entity->addComponent(ComponentType::NetworkSynced, 
		new NetworkSynced( entity->getIndex(), -1, EntityType::ShipModule));

	m_server->broadcastPacket(data.pack());
}