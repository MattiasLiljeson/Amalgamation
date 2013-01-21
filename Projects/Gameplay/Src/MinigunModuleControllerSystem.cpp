#include "MinigunModuleControllerSystem.h"
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
#include "EntityType.h"
#include "NetworkSynced.h"
#include <PhysicsController.h>

MinigunModuleControllerSystem::MinigunModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MinigunModuleControllerSystem, 1, ComponentType::MinigunModule)
{
	m_server = p_server;
}


MinigunModuleControllerSystem::~MinigunModuleControllerSystem()
{
}

void MinigunModuleControllerSystem::initialize()
{
}

void MinigunModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		MinigunModule* gun = static_cast<MinigunModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::MinigunModule)));

		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));

		if (gun && module && module->m_parentEntity >= 0)
		{
			handleLaserSight(p_entities[i]);

			//Update all rays
			for (unsigned int j = 0; j < 10; j++)
			{
				if (gun->rays[j].energy > 0)
				{
					AglVector3 dir = gun->rays[j].p2 - gun->rays[j].p1;
					dir.normalize();
					gun->rays[j].p1 += dir * 50 * dt;
					gun->rays[j].p2 += dir * 50 * dt;

					PhysicsSystem* physics = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
					int col = physics->getController()->FindClosestCollision(gun->rays[j].p1, gun->rays[j].p2);
					if (col >= 0)
					{
						gun->rays[j].energy -= 0.25f;
						physics->getController()->ApplyExternalImpulse(col, dir, AglVector3::zero());
					}
				}
			}


			//Check fire
			gun->coolDown = max(0, gun->coolDown - dt);
			if (gun->coolDown == 0 && module->m_active)
			{
				if (module->m_active)
					spawnRay(p_entities[i]);
			}
		}
	}
}
void MinigunModuleControllerSystem::handleLaserSight(Entity* p_entity)
{
	MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));

	if (gun->laserSightEntity < 0)
	{
		//Create Ray entity
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
void MinigunModuleControllerSystem::spawnRay(Entity* p_entity)
{
	MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));


	//Create Ray
	Transform* gunTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	Transform* t = new Transform(gunTransform->getTranslation(), AglQuaternion::identity(), AglVector3(0.2f, 0.2f, 0.2f));

	AglVector3 dir = gun->fireDirection;
	const AglQuaternion& rot = gunTransform->getRotation();
	rot.transformVector(dir);

	gun->coolDown = 0.1f;
	gun->rays[gun->currentRay].p1 = gunTransform->getTranslation() + dir * 1.5f;
	gun->rays[gun->currentRay].p2 = gunTransform->getTranslation() + dir * (1.5f + 50 * 0.1f);
	gun->rays[gun->currentRay].energy = 1.0f;
	gun->currentRay = (gun->currentRay+1) % 10;

	/*PhysicsBody* body = static_cast<PhysicsBody*>(p_entity->getComponent(ComponentType::PhysicsBody));

	PhysicsSystem* physics = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));
	AglVector3 vel = physics->getController()->getBody(body->m_id)->GetVelocity();


	Entity* entity = m_world->createEntity();

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(gunTransform->getTranslation(),
		AglQuaternion::identity(),
		AglVector3(0.2f, 0.2f, 0.2f), dir * 100.0f + vel, 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, false));

	entity->addComponent( ComponentType::Transform, t);
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

	m_server->broadcastPacket(data.pack());*/
}