#include "MineControllerSystem.h"
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
#include "StandardMine.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "SpawnSoundEffectPacket.h"

MineControllerSystem::MineControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::MineControllerSystem, 1, ComponentType::StandardMine)
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
		if (mine->m_age > 2)
		{
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));

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
//							hitModule->m_health = 0;
							hitModule->addDamageThisTick(101.0f); // Above max hp.
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
			}
		}	
	}
}