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
#include "ParticleSystemServerComponent.h"
#include "ShipManagerSystem.h"

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
			gun->cooldown = max(0, gun->cooldown - m_world->getDelta());
			gun->timeSinceRocket += dt;
			if (gun->cooldown == 0 && (module->getActive() || gun->currentBurst > 0) && gun->timeSinceRocket > 0.75f)
			{
				spawnRocket(p_entities[i],module);
				gun->timeSinceRocket = 0;
				gun->currentBurst++;
				if (gun->currentBurst >= gun->burstCount)
				{
					gun->lockCoolDown = 2.0f;
					gun->cooldown = 2.0f;
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


	Transform* trans = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	Entity* child = p_entity;
	if (module->m_parentEntity >= 0)
	{
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
				child = parent;
			}
		}

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(parent->getComponent(ComponentType::ConnectionPointSet));
		ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
			parent->getComponent(ComponentType::ShipConnectionPointHighlights) );

		for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
		{
			if (highlights->slotStatus[i])
			{
				if (cps->m_connectionPoints[i].cpConnectedEntity == child->getIndex())
				{
					AglVector3 target;
					Entity* ship = getClosestShip(p_entity, parent, target);

					if (ship || true)
					{
						ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
							p_entity->getComponent(ComponentType::ParticleSystemServerComponent));

						//Show the laser sight
						ps->getParticleSystemFromIdx(2)->updateData.color = ps->getParticleSystemFromIdx(2)->originalSettings.color;

						//Show the crosshair
						if (gun->cooldown > 0)
							ps->getParticleSystemFromIdx(0)->updateData.color = AglVector4(1, 1, 1, max(0, gun->cooldown-1.5f)*2.0f);
						else
						{
							ps->getParticleSystemFromIdx(0)->updateData.color = AglVector4(1, 1, 1, 1);
						}

						//Show some trailing smoke
						if (gun->cooldown > 1.8f)
							ps->getParticleSystemFromIdx(3)->updateData.spawnFrequency = ps->getParticleSystemFromIdx(3)->originalSettings.spawnFrequency;
						else
						{
							ps->getParticleSystemFromIdx(3)->updateData.spawnFrequency = 0;
						}


						if (gun->lockCoolDown == 0)
						{
							//Show the lockon
							ps->getParticleSystemFromIdx(1)->updateData.color = AglVector4(1, 1, 1, 1);
						}
						else
						{
							//Hide the lockon
							ps->getParticleSystemFromIdx(1)->updateData.color = AglVector4(0, 0, 0, 0);
						}
						ps->getParticleSystemFromIdx(0)->updateData.spawnPoint = target;
						ps->getParticleSystemFromIdx(1)->updateData.spawnPoint = target;
						if (ship)
						{
							gun->lockCoolDown = max(0, gun->lockCoolDown - m_world->getDelta());
							gun->target = ship->getIndex();
						}
						else
						{
							gun->lockCoolDown = 2.0f;
							gun->target = -1;
						}
					}

				}
				else
				{
					//Hide the crosshair
					ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
						p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
					ps->getParticleSystemFromIdx(0)->updateData.color = AglVector4(0, 0, 0, 0);
					ps->getParticleSystemFromIdx(0)->updateData.spawnPoint = trans->getTranslation();
					//Hide the lockon
					ps->getParticleSystemFromIdx(1)->updateData.color = AglVector4(0, 0, 0, 0);
					ps->getParticleSystemFromIdx(1)->updateData.spawnPoint = trans->getTranslation();

					//Hide the lasersight
					ps->getParticleSystemFromIdx(2)->updateData.color = AglVector4(0, 0, 0, 0);
				}
			}
		}
	}
	else
	{
		//Hide the crosshair
		//Hide the lockon
		//Hide the laser sight
		//Hide the smoke
		ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
			p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
		ps->getParticleSystemFromIdx(0)->updateData.color = AglVector4(0, 0, 0, 0);
		ps->getParticleSystemFromIdx(1)->updateData.color = AglVector4(0, 0, 0, 0);
		ps->getParticleSystemFromIdx(2)->updateData.color = AglVector4(0, 0, 0, 0);

		//Hide the smoke
		ps->getParticleSystemFromIdx(3)->updateData.spawnFrequency = 0;

		ps->getParticleSystemFromIdx(0)->updateData.spawnPoint = trans->getTranslation();
		ps->getParticleSystemFromIdx(1)->updateData.spawnPoint = trans->getTranslation();
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

	AglVector3 actualdir = t->getForward() * 100.0f + vel;
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
	rocketModule->m_target = gun->lockCoolDown == 0 ? gun->target : -1;
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

	//Explode particle effect
	ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
		p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
	ps->getParticleSystemFromIdx(4)->updateData.spawnFrequency = -1;

//	// Also send a positional sound effect.
//	SpawnSoundEffectPacket soundEffectPacket;
//	soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::MissileStartAndFlight;
//	soundEffectPacket.positional = true;
//	soundEffectPacket.position = t->getTranslation();
//	// NOTE: (Johan) Uncommented entity-sound because the entity id doesn't make sense.
//	soundEffectPacket.attachedToNetsyncEntity = -1; // entity->getIndex();
//	m_server->broadcastPacket(soundEffectPacket.pack());
}
Entity* RocketLauncherModuleControllerSystem::getClosestShip(Entity* p_entity, Entity* p_parentShip, AglVector3& p_target)
{
	//Calculate origin and direction of the ray
	Transform* shipTransform = static_cast<Transform*>(
		m_world->getComponentManager()->getComponent(p_parentShip,
		ComponentType::getTypeFor(ComponentType::Transform)));

	MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
		( p_parentShip->getComponent( ComponentType::MeshOffsetTransform) );

	//Fix!
	AglMatrix rayBase = meshOffset->offset*shipTransform->getMatrix();

	AglVector3 o = rayBase.GetTranslation();
	AglVector3 d = rayBase.GetForward();



	Transform* t = static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform));

	ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

	vector<Entity*> ships = sms->getShips();
	float minD = FLT_MAX;
	int id = -1;
	for (unsigned int i = 0; i < ships.size(); i++)
	{
		if (ships[i] != p_parentShip)
		{
			Transform* st = static_cast<Transform*>(ships[i]->getComponent(ComponentType::Transform));

			AglVector3 p = st->getTranslation();

			AglVector3 d2 = p-o;
			d2.normalize();

			if (AglVector3::dotProduct(d, d2) > 0.9f)
			{
				float dist = AglVector3::lengthSquared((o - p) - d*AglVector3::dotProduct(o-p, d));

				if (dist < minD)
				{
					id = i;
					minD = dist;
				}
			}
		}

	}
	if (id >= 0)
	{
		Transform* shipTransform = static_cast<Transform*>(
			m_world->getComponentManager()->getComponent(ships[id],
			ComponentType::getTypeFor(ComponentType::Transform)));
		p_target = shipTransform->getTranslation();
		return ships[id];
	}

	p_target = o + d * 200;
	return NULL;
}
