#include "MinigunModuleControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include <Control.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "EntityCreationPacket.h"
#include "EntityType.h"
#include "NetworkSynced.h"
#include <PhysicsController.h>
#include "ParticleUpdatePacket.h"
#include "ParticleSystemServerComponent.h"
#include "SpawnSoundEffectPacket.h"
#include "ParticleSystemCreationInfo.h"
#include "ParticleEmitters.h"

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

		handleParticleSystem(p_entities[i]);
		handleLaserSight(p_entities[i]);
		if (gun && module && module->m_parentEntity >= 0)
		{
			//Update all rays
			for (unsigned int j = 0; j < MinigunModule::rayCount; j++)
			{
				if (gun->rays[j].energy > 0)
				{
					AglVector3 dir = gun->rays[j].p2 - gun->rays[j].p1;
					dir.normalize();
					gun->rays[j].p1 += dir * gun->range * dt;
					gun->rays[j].p2 += dir * gun->range * dt;

					PhysicsSystem* physics = static_cast<PhysicsSystem*>
						(m_world->getSystem(SystemType::SystemTypeIdx::PhysicsSystem));

					PhysicsBody* body = static_cast<PhysicsBody*>(
						m_world->getComponentManager()->getComponent(p_entities[i],
						ComponentType::getTypeFor(ComponentType::PhysicsBody)));

					int col = physics->getController()->FindClosestCollision(
						gun->rays[j].p1, gun->rays[j].p2, body->m_id);
					if (col >= 0)
					{
						gun->rays[j].energy -= 0.25f * dt;
						Entity* hitE = physics->getEntity(col);

						if (hitE)
						{
							ShipModule* hitModule = static_cast<ShipModule*>(hitE->getComponent(ComponentType::ShipModule));
							if (hitModule && hitModule != module)
							{
								hitModule->m_health -= 100 * dt;
							}
						}
					}
				}
			}


			//Check fire
			gun->coolDown = max(0, gun->coolDown - dt);
			if (gun->coolDown == 0 && module->getActive())
			{
				spawnRay(p_entities[i]);
			}
			updateRays(p_entities[i]);
		}
	}
}
void MinigunModuleControllerSystem::handleLaserSight(Entity* p_entity)
{
	MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));

	ShipModule* module = static_cast<ShipModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ShipModule)));

	if (gun->laserSightEntity < 0)
	{
		//Create Ray entity
		Entity* entity = m_world->createEntity();

		Transform* t = new Transform(
			AglVector3(0, 0, 0),
			AglQuaternion::rotateToFrom(AglVector3(0, 0, 1),
			gun->fireDirection), AglVector3(0.03f, 0.03f, 20));

		entity->addComponent( ComponentType::Transform, t);
		m_world->addEntity(entity);
		gun->laserSightEntity = entity->getIndex();

		EntityCreationPacket data;
		data.entityType		= static_cast<char>(EntityType::LaserSight);
		data.owner			= -1;
		data.networkIdentity = entity->getIndex();
		data.translation	= t->getTranslation();
		data.rotation		= t->getRotation();
		data.scale			= t->getScale();

		entity->addComponent(ComponentType::NetworkSynced, 
			new NetworkSynced( entity->getIndex(), -1, EntityType::LaserSight));

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

		if (module->m_parentEntity >= 0)
		{
			AglQuaternion rot = gunTransform->getRotation()*AglQuaternion::rotateToFrom(
				AglVector3(0, 0, 1), gun->fireDirection);

			AglVector3 offset = AglVector3(0.03f, 0.03f, 20.0f);
			rot.transformVector(offset);
			laserTransform->setTranslation(gunTransform->getTranslation()+offset);
			laserTransform->setRotation(rot);


			//Check if the module is highlighted
			Entity* parent = NULL;
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
					p_entity = parent;
				}
			}

			ConnectionPointSet* cps = static_cast<ConnectionPointSet*>
				(parent->getComponent(ComponentType::ConnectionPointSet));
			if (cps->m_connectionPoints[cps->m_highlighted].cpConnectedEntity == p_entity->getIndex())
				laserTransform->setScale(AglVector3(0.03f, 0.03f, 20));
			else
				laserTransform->setScale(AglVector3(0, 0, 0));
		}
		else
		{
			laserTransform->setScale(AglVector3(0, 0, 0));
		}
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

	AglVector3 dir = gun->fireDirection;
	const AglQuaternion& rot = gunTransform->getRotation();
	rot.transformVector(dir);

	gun->coolDown = 1.0f / MinigunModule::rayCount;
	gun->rays[gun->currentRay].p1 = gunTransform->getTranslation();
	gun->rays[gun->currentRay].p2 = gunTransform->getTranslation()
		+ dir * gun->range / MinigunModule::rayCount;
	gun->rays[gun->currentRay].energy = 1.0f;
	gun->currentRay = (gun->currentRay+1) % MinigunModule::rayCount;


	//Send a sound
	SpawnSoundEffectPacket soundEffectPacket;
	soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::LaserGun;
	soundEffectPacket.positional = true;
	soundEffectPacket.position = gunTransform->getTranslation();
	soundEffectPacket.attachedToNetsyncEntity = -1; // entity->getIndex();
	m_server->broadcastPacket(soundEffectPacket.pack());
}
void MinigunModuleControllerSystem::updateRays(Entity* p_entity)
{
	/*MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));


	//Create Ray
	Transform* gunTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	for (unsigned int i = 0; i < MinigunModule::rayCount; i++)
	{
		if (gun->rays[i].entityId < 0)
		{
			Entity* entity = m_world->createEntity();
			Transform* t = new Transform();
			entity->addComponent( ComponentType::Transform, t);
			m_world->addEntity(entity);

			EntityCreationPacket data;
			data.entityType		= static_cast<char>(EntityType::ShipModule);
			data.owner			= -1;
			data.networkIdentity = entity->getIndex();
			data.translation	= t->getTranslation();
			data.rotation		= t->getRotation();
			data.scale			= t->getScale();
			data.meshInfo		= 0;

			entity->addComponent(ComponentType::NetworkSynced, 
				new NetworkSynced( entity->getIndex(), -1, EntityType::ShipModule));

			m_server->broadcastPacket(data.pack());
			gun->rays[i].entityId = entity->getIndex();
		}
		else
		{
			Entity* e = m_world->getEntity(gun->rays[i].entityId);
			Transform* trans = static_cast<Transform*>(
				m_world->getComponentManager()->getComponent(e,
				ComponentType::getTypeFor(ComponentType::Transform)));
			if (gun->rays[i].energy > 0)
				trans->setScale(AglVector3(0.1f, 0.1f, gun->range / (MinigunModule::rayCount*2)));
			else
				trans->setScale(AglVector3(0.0f, 0.0f, 0.0f));
			trans->setTranslation((gun->rays[i].p1 + gun->rays[i].p2)*0.5f);

			AglVector3 dir = gun->rays[i].p2 - gun->rays[i].p1;
			dir.normalize();
			trans->setRotation(AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), dir));

		}
	}*/
}
void MinigunModuleControllerSystem::handleParticleSystem(Entity* p_entity)
{
	MinigunModule* gun = static_cast<MinigunModule*>
		( p_entity->getComponent( ComponentType::MinigunModule) );

	if (gun && gun->particleSystemEntity < 0)
	{
		//Entity* entity = m_world->createEntity();
		//
		//entity->addComponent( ComponentType::ParticleUpdateData, new ParticleUpdateData());
		//
		//EntityCreationPacket data;
		//data.entityType			= static_cast<char>(EntityType::ParticleSystem);
		//data.owner				= -1;
		//data.networkIdentity		= entity->getIndex();
		//data.translation			= AglVector3(0, 0, 0);
		//data.rotation				= AglQuaternion::identity();
		//data.scale				= AglVector3(0, 0, 0);
		//data.meshInfo				= 0;
		//
		//entity->addComponent( new NetworkSynced( entity->getIndex(), -1,
		//	EntityType::ParticleSystem ) );
		//
		//m_server->broadcastPacket(data.pack());
		//m_world->addEntity(entity);

		EntityCreationPacket creationPacket;
		creationPacket.entityType			= EntityType::MinigunModule;
		creationPacket.owner				= -1;
		creationPacket.networkIdentity		= p_entity->getIndex();
		creationPacket.translation			= AglVector3( 0, 0, 0 );
		creationPacket.rotation				= AglQuaternion::identity();
		creationPacket.scale				= AglVector3( 0, 0, 0 );
		creationPacket.meshInfo				= 0;
		m_server->broadcastPacket( creationPacket.pack() );
		
		//p_entity->addComponent( new NetworkSynced( p_entity->getIndex(), -1,
		//	EntityType::ParticleSystem ) );

		//ParticleSystemCreationInfo psInfo;
		//psInfo.entityNetId = p_entity->getIndex();
		//psInfo.particleSysIdx = particleSysIdx;
		//psInfo.particleSysHeader = header;

		//Packet packet( PacketType::ParticleSystemCreationInfo );
		//packet.WriteData( (char*)&psInfo, sizeof( ParticleSystemCreationInfo ) );
		//m_server->broadcastPacket( packet );

		gun->particleSystemEntity = p_entity->getIndex(); // Itself
	}
	else if( gun )
	{
		//Entity* entity = m_world->getEntity(gun->particleSystemEntity);
		
		PhysicsSystem* physics = static_cast<PhysicsSystem*>
			( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

		PhysicsBody* body = static_cast<PhysicsBody*>
			( p_entity->getComponent( ComponentType::PhysicsBody) );

		Transform* parentTrans = static_cast<Transform*>
			( p_entity->getComponent( ComponentType::Transform) );

		AglVector3 dir = gun->fireDirection;
		parentTrans->getRotation().transformVector( dir );

		AglVector3 vel = physics->getController()->getBody( body->m_id )->GetVelocity();
		vel += dir * 50; // Why 50? /ML

		ParticleSystemServerComponent* particleComp = static_cast<ParticleSystemServerComponent*>
			( p_entity->getComponent( ComponentType::ParticleSystemServerComponent) );
		if( particleComp != NULL )
		{
			ParticleSystemData* data = particleComp->getParticleSystemDataPtrFromName( "minigun" );
			data->updateData.speed = vel.length();
			vel.normalize();

			ShipModule* module = static_cast<ShipModule*>
				( p_entity->getComponent( ComponentType::ShipModule ) );
			data->updateData.spawnPoint = parentTrans->getTranslation();
			data->updateData.direction = vel;
			data->updateData.spawnFrequency = ( (float)module->getActive() ) * 10;//200; // Why 10? /ML
		}
	}

}