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
#include "ParticleSystemsComponent.h"
#include "ShipConnectionPointHighlights.h"
#include "ModuleHelper.h"
#include "SpawnPointSet.h"
#include "MeshOffsetTransform.h"
#include "AnimationUpdatePacket.h"
#include "SoundPacket.h"

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

		gun->timeSinceSound += dt;

		ShipModule* module = static_cast<ShipModule*>(
			p_entities[i]->getComponent(ComponentType::ShipModule));

		PhysicsBody* body = static_cast<PhysicsBody*>(
			p_entities[i]->getComponent(ComponentType::PhysicsBody));

		// get owner for damage set
		int ownerId = ModuleHelper::FindParentShipClientId(m_world, module);

		AglVector3 rayDir;
		AglVector3 rayPos;

		updateRay(p_entities[i], rayPos, rayDir);

		handleLaserSight(p_entities[i], rayPos, rayDir);

		if (gun && module && module->m_parentEntity >= 0)
		{
			PhysicsSystem* physics = static_cast<PhysicsSystem*>
				( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

			//Check fire
			if (module->getActive())
			{
				if (!gun->animationPlaying)
				{
					//Start playing animation
					startAnimation(p_entities[i]);
					gun->animationPlaying = true;
				}

				//Send a sound
				if (gun->timeSinceSound > 0.08f)
				{
					SpawnSoundEffectPacket soundEffectPacket;
					soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::LaserGun;
					soundEffectPacket.positional = true;
					soundEffectPacket.position = rayPos;
					soundEffectPacket.attachedToNetsyncEntity = -1; // entity->getIndex();
					m_server->broadcastPacket(soundEffectPacket.pack());
					gun->timeSinceSound = 0;
				}


				//Check if we hit something
				if (gun->rayIndex >= 0)
				{
					int col = physics->getController()->LineClosestCollision(gun->rayIndex, body->m_id);
					if (col >= 0)
					{
						Entity* collided = physics->getEntity(col);
						if(collided)
						{
							ShipModule* colModule = static_cast<ShipModule*>(
								collided->getComponent(ComponentType::ShipModule));

							if (colModule)
							{
								colModule->addDamageThisTick(200*dt, ownerId);

								AglVector3 o;
								AglVector3 d;
								physics->getController()->GetRay(gun->rayIndex, o, d);
								physics->getController()->ApplyExternalImpulse(col, d*dt*3.0f, AglVector3(0, 0, 0));
							}
						}

						return;
					}
				}
			}
			else if (!module->getActive())
			{
				if (gun->animationPlaying)
				{
					//Stop playing animation
					stopAnimation(p_entities[i]);
					gun->animationPlaying = false;
				}
				//Stop the fire animation
				ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
					p_entities[i]->getComponent(ComponentType::ParticleSystemServerComponent));
				ps->getParticleSystemFromIdx(0)->updateData.spawnFrequency = 0;
				ps->getParticleSystemFromIdx(1)->updateData.spawnFrequency = 0;
			}
		}
		else
		{
			//Stop the fire animation
			ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
				p_entities[i]->getComponent(ComponentType::ParticleSystemServerComponent));
			ps->getParticleSystemFromIdx(0)->updateData.spawnFrequency = 0;
			ps->getParticleSystemFromIdx(1)->updateData.spawnFrequency = 0;
		}
	}
}
void MinigunModuleControllerSystem::handleLaserSight(Entity* p_entity, AglVector3 p_rayPos, AglVector3 p_rayDir)
{
	MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));

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

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>
			(parent->getComponent(ComponentType::ConnectionPointSet));
		ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
			parent->getComponent(ComponentType::ShipConnectionPointHighlights) );

		for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
		{
			if (highlights->slotStatus[i])
			{
				if (cps->m_connectionPoints[i].cpConnectedEntity == child->getIndex())
				{
					ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
						p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
					if (ps)
					{
						PhysicsSystem* physics = static_cast<PhysicsSystem*>
							( m_world->getSystem( SystemType::SystemTypeIdx::PhysicsSystem ) );

						PhysicsBody* body = static_cast<PhysicsBody*>(
							p_entity->getComponent(ComponentType::PhysicsBody));

						AglVector3 colPoint;
						int col = physics->getController()->LineClosestCollision(gun->rayIndex, colPoint, body->m_id);

						ps->getParticleSystemFromIdx(2)->updateData.color = ps->getParticleSystemFromIdx(2)->originalSettings.color;
						ps->getParticleSystemFromIdx(2)->updateData.spawnFrequency = ps->getParticleSystemFromIdx(0)->originalSettings.spawnFrequency;
						ps->getParticleSystemFromIdx(3)->updateData.color = AglVector4(0, 0, 0, 0);
						if (col >= 0)
							ps->getParticleSystemFromIdx(3)->updateData.color = AglVector4(1, 0, 0, 1);
						ps->getParticleSystemFromIdx(3)->updateData.spawnPoint = colPoint;
					}
				}
				else
				{
					ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
						p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
					if (ps)
					{
						ps->getParticleSystemFromIdx(2)->updateData.spawnFrequency = 0;
						ps->getParticleSystemFromIdx(2)->updateData.color = AglVector4(0, 0, 0, 0);
						ps->getParticleSystemFromIdx(3)->updateData.color = AglVector4(0, 0, 0, 0);
						ps->getParticleSystemFromIdx(3)->updateData.spawnPoint = trans->getTranslation();
					}
				}
			}
		}
	}
	else
	{
		ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
			p_entity->getComponent(ComponentType::ParticleSystemServerComponent));
		if (ps)
		{
			ps->getParticleSystemFromIdx(2)->updateData.spawnFrequency = 0;
			ps->getParticleSystemFromIdx(2)->updateData.color = AglVector4(0, 0, 0, 0);
			ps->getParticleSystemFromIdx(3)->updateData.color = AglVector4(0, 0, 0, 0);
			ps->getParticleSystemFromIdx(3)->updateData.spawnPoint = trans->getTranslation();
		}
	}
}

void MinigunModuleControllerSystem::startAnimation(Entity* p_gun)
{
	AnimationUpdatePacket packet;
	packet.networkIdentity = p_gun->getIndex();
	packet.shouldPlay = true;
	packet.playSpeed = 15.0f;
	packet.take = "Shooting";
	m_server->broadcastPacket( packet.pack() );


	//Play the fire animation
	ParticleSystemServerComponent* ps = static_cast<ParticleSystemServerComponent*>(
											p_gun->getComponent(ComponentType::ParticleSystemServerComponent));
	ps->getParticleSystemFromIdx(0)->updateData.spawnFrequency = ps->getParticleSystemFromIdx(0)->originalSettings.spawnFrequency;
	ps->getParticleSystemFromIdx(1)->updateData.spawnFrequency = ps->getParticleSystemFromIdx(1)->originalSettings.spawnFrequency;

	//Start playing the minigun fire sound
	SoundPacket sp;
	sp.target = p_gun->getIndex();
	sp.soundType = AudioHeader::AMBIENT;
	sp.targetSlot = 0;
	sp.queuedPlayingState = AudioHeader::PLAY;
	m_server->broadcastPacket( sp.pack() );
}
void MinigunModuleControllerSystem::stopAnimation(Entity* p_gun)
{
	AnimationUpdatePacket packet;
	packet.networkIdentity = p_gun->getIndex();
	packet.shouldPlay = true;
	packet.take = "Default";
	m_server->broadcastPacket( packet.pack() );

	//Stop playing the minigun fire sound
	SoundPacket sp;
	sp.target = p_gun->getIndex();
	sp.soundType = AudioHeader::AMBIENT;
	sp.targetSlot = 0;
	sp.queuedPlayingState = AudioHeader::STOP;
	m_server->broadcastPacket( sp.pack() );
}
void MinigunModuleControllerSystem::updateRay(Entity* p_entity, AglVector3& p_rayPos, AglVector3& p_rayDir)
{
	MinigunModule* gun = static_cast<MinigunModule*>(
		m_world->getComponentManager()->getComponent(p_entity,
		ComponentType::getTypeFor(ComponentType::MinigunModule)));

	//Modify ray
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
		if (sps->m_spawnPoints[sp].spAction == "Laser")
		{
			sightOffset = sps->m_spawnPoints[sp].spTransform;
			break;
		}
	}

	Transform newTransform = Transform(sightOffset*meshOffset->offset*body->getOffset().inverse()*rigidBody->GetWorld());

	if (gun->rayIndex >= 0)
		physics->getController()->SetRay(newTransform.getTranslation(), newTransform.getForward(), gun->rayIndex);
	else
	{
		gun->rayIndex = physics->getController()->AddRay(newTransform.getTranslation(), newTransform.getForward());
	}
	p_rayDir = newTransform.getForward();
	p_rayPos = newTransform.getTranslation();
}