#include "RocketControllerSystem.h"
#include "ShipManagerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>

RocketControllerSystem::RocketControllerSystem()
	: EntitySystem(SystemType::RocketControllerSystem, 1, ComponentType::StandardRocket)
{
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

			ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);
		}	
	}
}