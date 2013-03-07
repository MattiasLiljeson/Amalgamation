#include "RocketControllerSystem.h"
#include "ShipManagerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "SpawnSoundEffectPacket.h"
#include "ShipModule.h"
#include "ParticleSystemCreationInfo.h"
#include "SpawnExplosionPacket.h"
#include "MeshOffsetTransform.h"

RocketControllerSystem::RocketControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::RocketControllerSystem, 3, ComponentType::StandardRocket,
	ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_turnPower = 8.0f;
	m_enginePower = 150.0f;
	m_server = p_server;
}


RocketControllerSystem::~RocketControllerSystem()
{
}

void RocketControllerSystem::initialize()
{
}

void RocketControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();
	float waitUntilActivation = 0.1f;
	float rocketMaxAge = 15.0f;
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		StandardRocket* rocket = static_cast<StandardRocket*>(p_entities[i]->getComponent(ComponentType::StandardRocket));

		if (rocket->m_age == 0)
		{
			//Align with movement direction
			MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
				( p_entities[i]->getComponent( ComponentType::MeshOffsetTransform) );

			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
			AglMatrix world = body->GetWorld();

			world = meshOffset->offset.inverse()*world;
			world = pb->getOffset()*world;
			body->setTransform(world);
		}

		rocket->m_age += dt;
		//Check collision
		if (rocket->m_age > waitUntilActivation && rocket->m_age <= rocketMaxAge)
		{
			//Start targeting ships
			ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

			Entity* ship = m_world->getEntity(rocket->m_target);

			Transform* from = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
			Transform* to = static_cast<Transform*>(ship->getComponent(ComponentType::Transform));

			//START APPLY IMPULSE

			MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
				( p_entities[i]->getComponent( ComponentType::MeshOffsetTransform) );

			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));

			AglVector3 imp = to->getTranslation() - from->getTranslation();

			float distance = imp.length();

			imp.normalize();
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));

			AglMatrix world = meshOffset->offset * pb->getOffset().inverse() * body->GetWorld();

			AglVector3 rotAxis = AglVector3::crossProduct(imp, -world.GetForward());
			rotAxis.normalize();

			//Compute fraction of turn power that should be applied
			AglVector3 angVel = body->GetAngularVelocity();
			
			float angVelAxis = max(AglVector3::dotProduct(angVel, rotAxis), 0);

			float amountToRotate = min(1.0f - AglVector3::dotProduct(imp, world.GetForward()), 1.0f); //0 -> 1

			float frac = 1.0f;
			if (amountToRotate / angVelAxis < 0.25f)
			{
				frac = (amountToRotate / angVelAxis) / 0.25f;
			}

			rotAxis *= m_turnPower * dt * frac;

			if (true)//distance < 100)
			{
				frac = 1-frac;
				
				/*if (distance < 100)
				{
					frac *= (0.5f + distance / 100.0f);
				}*/
			}
			else
			{
				frac = 1.0f;
			}

			AglVector3 impulse = imp*0.25f + world.GetForward();
			impulse.normalize();
			impulse *= dt*m_enginePower*frac;

			ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);

			//END APPLY IMPULSE

			//Check collision	
			vector<unsigned int> cols = ps->getController()->CollidesWith(pb->m_id);
			if (cols.size() > 0)
			{
				// Apply damage to first found collision
				Entity* hitEntity = ps->getEntity(cols[0]);
				if(hitEntity)
				{
					ShipModule* hitModule = static_cast<ShipModule*>(hitEntity->getComponent(
						ComponentType::ShipModule));
					StandardRocket* hitRocket = static_cast<StandardRocket*>(hitEntity->getComponent(
						ComponentType::StandardRocket));
					if(hitRocket==NULL)
					{
						if (hitModule)
						{
							hitModule->addDamageThisTick(101.0f,rocket->m_ownerId); // Above max hp.
						}
						explodeRocket(ps, pb, body, p_entities[i]);
					}
				}
			}
		}// if (rocket->m_age > waitUntilActivation && rocket->m_age <= rocketMaxAge)
		else if(rocket->m_age > rocketMaxAge)
		{
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
			explodeRocket(ps, pb, body, p_entities[i]);
		}
	}
}

void RocketControllerSystem::explodeRocket(PhysicsSystem* p_physicsSystem,
	PhysicsBody* p_physicsBody, RigidBody* p_rigidBody, Entity* p_entity)
{
	// Remove the rocket...
	p_physicsSystem->getController()->ApplyExternalImpulse(p_rigidBody->GetWorld().GetTranslation(), 20, 20);
	p_physicsSystem->getController()->InactivateBody(p_physicsBody->m_id);
	
	m_world->deleteEntity(p_entity);
	// ...and play an explosion sound effect.
	Transform* t = static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform));
	SpawnSoundEffectPacket soundEffectPacket;
	soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::Explosion;
	soundEffectPacket.positional = true;
	soundEffectPacket.position = t->getTranslation();
	soundEffectPacket.attachedToNetsyncEntity = -1;
	m_server->broadcastPacket(soundEffectPacket.pack());


	SpawnExplosionPacket explosion;
	explosion.position = t->getTranslation();
	m_server->broadcastPacket(explosion.pack());
}


//Old funny fish
//Start targeting ships
/*ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

//TEMP: Target first ship in array
Entity* ship = shipManager->getShips()[0];

Transform* from = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
Transform* to = static_cast<Transform*>(ship->getComponent(ComponentType::Transform));

PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));

AglVector3 imp = to->getTranslation() - from->getTranslation();
imp.normalize();
RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
AglVector3 rotAxis = AglVector3::crossProduct(imp, body->GetWorld().GetForward());
rotAxis.normalize();
float rotFraction = (1.0f-max(AglVector3::dotProduct(imp, -body->GetWorld().GetForward()), 0.0f));
rotAxis *= 10.0f * rotFraction * dt;

AglVector3 impulse = -body->GetWorld().GetForward()*dt*50 * (1.0f - rotFraction);

ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);*/