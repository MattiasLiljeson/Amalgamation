#include "MineControllerSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "..\..\Input\Src\Control.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "StandardMine.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>

MineControllerSystem::MineControllerSystem()
	: EntitySystem(SystemType::MineControllerSystem, 1, ComponentType::StandardMine)
{
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
				ps->getController()->ApplyExternalImpulse(b->GetWorld().GetTranslation(), 300);
				mine->m_age = 0;
			}
		}	
	}
}