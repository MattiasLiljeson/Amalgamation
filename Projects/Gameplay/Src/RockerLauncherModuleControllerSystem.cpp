#include "RocketLauncherModuleControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
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
#include "SpawnSoundEffectPacket.h"
#include "ShipConnectionPointHighlights.h"
#include "ModuleHelper.h"
#include "SpawnPointSet.h"
#include "MeshOffsetTransform.h"
#include "LoadMesh.h"

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

		handleLaserSight(p_entities[i]);
		if (gun && module && module->m_parentEntity >= 0)
		{
			//Check fire
			gun->coolDown = max(0, gun->coolDown - dt);
			gun->timeSinceRocket += dt;
			if (gun->coolDown == 0 && (module->getActive() || gun->currentBurst > 0) && gun->timeSinceRocket > 0.75f)
			{
				spawnRocket(p_entities[i],module);
				gun->timeSinceRocket = 0;
				gun->currentBurst++;
				if (gun->currentBurst >= gun->burstCount)
				{
					gun->coolDown = 1.0f;
					gun->currentBurst = 0;
				}
			}
		}
	}
}

void RocketLauncherModuleControllerSystem::handleLaserSight(Entity* p_entity)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	ShipModule* module = static_cast<ShipModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::ShipModule)));

	if (gun->laserSightEntity < 0)
	{
		//Create Ray entity
		Entity* entity = m_world->createEntity();

		Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection), AglVector3(0.03f, 0.03f, 20));
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
			AglQuaternion rot = gunTransform->getRotation()*AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), gun->fireDirection);

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

			ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(parent->getComponent(ComponentType::ConnectionPointSet));
			ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
				parent->getComponent(ComponentType::ShipConnectionPointHighlights) );

			for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
			{
				if (highlights->slotStatus[i])
				{
					if (cps->m_connectionPoints[i].cpConnectedEntity == p_entity->getIndex())
						laserTransform->setScale(AglVector3(0.03f, 0.03f, 20));
					else
						laserTransform->setScale(AglVector3(0, 0, 0));
				}
			}
		}
		else
		{
			laserTransform->setScale(AglVector3(0, 0, 0));
		}
	}
}
void RocketLauncherModuleControllerSystem::spawnRocket(Entity* p_entity,ShipModule* p_module)
{
	RocketLauncherModule* gun = static_cast<RocketLauncherModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::RocketLauncherModule)));

	//Create Rocket
	Transform* gunTransform = static_cast<Transform*>(
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
	AglMatrix sightOffset = AglMatrix::identityMatrix();
	for (unsigned int sp = 0; sps->m_spawnPoints.size(); sp++)
	{
		if (sps->m_spawnPoints[sp].spAction == "rocket")
		{
			sightOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	//Fix!
	Transform newTransform = Transform(sightOffset*meshOffset->offset*body->getOffset().inverse()*rigidBody->GetWorld());

	Transform* t = new Transform(newTransform.getMatrix());

	AglVector3 vel = physics->getController()->getBody(body->m_id)->GetVelocity();


	Entity* entity = m_world->createEntity();

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	AglVector3 actualdir = t->getForward() * 200.0f + vel;
	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(gunTransform->getTranslation(),
		AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), t->getForward()),
		AglVector3(1.0f, 1.0f, 1.0f), actualdir, 
		AglVector3(0, 0, 0), 3, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false, true));

	entity->addComponent( ComponentType::Transform, t);

	entity->addComponent(ComponentType::LoadMesh, new LoadMesh("rocket.agl"));


	// store owner data
	StandardRocket* rocketModule = new StandardRocket();
	rocketModule->m_ownerId = ModuleHelper::FindParentShipClientId(m_world, p_module);

	entity->addComponent(ComponentType::StandardRocket, rocketModule);

	entity->addComponent(ComponentType::NetworkSynced, 
		new NetworkSynced( entity->getIndex(), -1, EntityType::Rocket));
	m_world->addEntity(entity);

	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::Rocket);
	data.owner			= -1;
	data.networkIdentity = entity->getIndex();
	data.translation	= t->getTranslation();
	data.rotation		= t->getRotation();
	data.scale			= t->getScale();
	data.meshInfo		= 1;
	m_server->broadcastPacket(data.pack());

//	// Also send a positional sound effect.
//	SpawnSoundEffectPacket soundEffectPacket;
//	soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::MissileStartAndFlight;
//	soundEffectPacket.positional = true;
//	soundEffectPacket.position = t->getTranslation();
//	// NOTE: (Johan) Uncommented entity-sound because the entity id doesn't make sense.
//	soundEffectPacket.attachedToNetsyncEntity = -1; // entity->getIndex();
//	m_server->broadcastPacket(soundEffectPacket.pack());
}
