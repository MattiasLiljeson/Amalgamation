#include "RocketControllerSystem.h"
#include "ShipManagerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "SpawnSoundEffectPacket.h"
#include "ShipModule.h"

RocketControllerSystem::RocketControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::RocketControllerSystem, 3, ComponentType::StandardRocket,
	ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_turnPower = 2.0f;
	m_enginePower = 50.0f;
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

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		StandardRocket* rocket = static_cast<StandardRocket*>(p_entities[i]->getComponent(ComponentType::StandardRocket));
		rocket->m_age += dt;

		//Check collision
		if (rocket->m_age > 0.2f)
		{
			//Start targeting ships
			ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

			Entity* ship = shipManager->getShips()[0];

			Transform* from = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
			Transform* to = NULL;

			//Find Closest Target
			float dist = FLT_MAX;
			vector<Entity*> ships = shipManager->getShips();
			for (unsigned int s = 0; s < ships.size(); s++)
			{
				if (ships[s] != p_entities[i])
				{
					to = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
					if (AglVector3::lengthSquared(to->getTranslation()-from->getTranslation()) < dist)
					{
						dist = AglVector3::lengthSquared(to->getTranslation()-from->getTranslation());
						ship = ships[s];
					}
				}
			}


			to = static_cast<Transform*>(ship->getComponent(ComponentType::Transform));

			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));

			AglVector3 imp = to->getTranslation() - from->getTranslation();
			imp.normalize();
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
			AglVector3 rotAxis = AglVector3::crossProduct(imp, body->GetWorld().GetForward());
			rotAxis.normalize();
			float rotFraction = (max(AglVector3::dotProduct(imp, -body->GetWorld().GetForward()), 0.0f));
			rotAxis *= m_turnPower * dt;

			AglVector3 impulse = -body->GetWorld().GetForward()*dt*m_enginePower;

			ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);



			//Check collision	
			vector<unsigned int> cols = ps->getController()->CollidesWith(pb->m_id);
			if (cols.size() > 0)
			{
				ps->getController()->ApplyExternalImpulse(body->GetWorld().GetTranslation(), 20, 20);
				ps->getController()->InactivateBody(pb->m_id);

				// Apply damage to first found collision
				Entity* hitEntity = ps->getEntity(cols[0]);
				if(hitEntity)
				{
					ShipModule* hitModule = static_cast<ShipModule*>(hitEntity->getComponent(
						ComponentType::ShipModule));
					if(hitModule)
					{
//						hitModule->m_health = 0;
						hitModule->addDamageThisTick(101.0f); // Above max hp.
					}
				}

				//Send an explosion sound effect
				Transform* t = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));

				SpawnSoundEffectPacket soundEffectPacket;
				soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::Explosion;
				soundEffectPacket.positional = true;
				soundEffectPacket.position = t->getTranslation();
				soundEffectPacket.attachedToNetsyncEntity = -1;
				m_server->broadcastPacket(soundEffectPacket.pack());

				m_world->deleteEntity(p_entities[i]);
			}
		}
	}
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