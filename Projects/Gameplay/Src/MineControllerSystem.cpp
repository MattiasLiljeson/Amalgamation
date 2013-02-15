#include "MineControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "StandardMine.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "SpawnSoundEffectPacket.h"
#include "ShipManagerSystem.h"

MineControllerSystem::MineControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MineControllerSystem, 3, ComponentType::StandardMine,
	ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_server = p_server;
}


MineControllerSystem::~MineControllerSystem()
{
}

void MineControllerSystem::initialize()
{
}

void MineControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		StandardMine* mine = static_cast<StandardMine*>(p_entities[i]->getComponent(ComponentType::StandardMine));
		mine->m_age += dt;

		//Check collision
		if (mine->m_age > 2.0f)
		{
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(
				m_world->getSystem(SystemType::ShipManagerSystem));
			StandardMine* mine = static_cast<StandardMine*>(p_entities[i]->
				getComponent(ComponentType::StandardMine));
			for(unsigned int ship=0; ship<shipManager->getShips().size(); ship++)
			{
				Transform* mineTransform = static_cast<Transform*>(p_entities[i]->
					getComponent(ComponentType::Transform));
				Transform* shipTransform = static_cast<Transform*>(
					shipManager->getShips()[ship]->getComponent(ComponentType::Transform));
				if(shipTransform != NULL)
				{
					AglVector3 distVector = shipTransform->getTranslation() -
						mineTransform->getTranslation();
					if(distVector.lengthSquared() < mine->range * mine->range)
					{
						distVector.normalize();
						float impulseFactor = 0.01f * m_world->getDelta() * (mine->range * mine->range) /
							distVector.lengthSquared();
						ps->getController()->ApplyExternalImpulse(pb->m_id,
							distVector * impulseFactor, AglVector3::zero());
					}
				}
			}

			vector<unsigned int> col = ps->getController()->CollidesWith(pb->m_id);
			Body* b = ps->getController()->getBody(pb->m_id);
			if (mine->m_age > 2 && col.size() > 0)
			{
				//Do some damage
				for (unsigned int j = 0; j < col.size(); j++)
				{
					Entity* hitEntity = ps->getEntity(col[j]);
					if (hitEntity)
					{
						ShipModule* hitModule = static_cast<ShipModule*>(hitEntity->getComponent(ComponentType::ShipModule));
						if (hitModule)
						{
							hitModule->addDamageThisTick(101.0f,mine->m_ownerId); // Above max hp.
						}
					}
				}

				//Send a shockwave
				ps->getController()->ApplyExternalImpulse(b->GetWorld().GetTranslation(), 20, 20);
				ps->getController()->InactivateBody(pb->m_id);

				//Send an explosion sound effect
				Transform* t = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
				SpawnSoundEffectPacket soundEffectPacket;
				soundEffectPacket.soundIdentifier = (int)SpawnSoundEffectPacket::Explosion;
				soundEffectPacket.positional = true;
				soundEffectPacket.position = t->getTranslation();
				soundEffectPacket.attachedToNetsyncEntity = -1; // entity->getIndex();
				m_server->broadcastPacket(soundEffectPacket.pack());

				m_world->deleteEntity(p_entities[i]);
			}
		}	
	}
}