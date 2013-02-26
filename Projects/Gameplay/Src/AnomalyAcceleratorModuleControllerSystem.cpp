#include "AnomalyAcceleratorModuleControllerSystem.h"
#include "ShipModule.h"
#include "AnomalyAcceleratorModule.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>

AnomalyAcceleratorModuleControllerSystem::AnomalyAcceleratorModuleControllerSystem()
	: EntitySystem(SystemType::AnomalyAcceleratorModuleControllerSystem, 4,
	ComponentType::AnomalyAcceleratorModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::ShipModule)
{
}

void AnomalyAcceleratorModuleControllerSystem::processEntities(
	const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			p_entities[i]->getComponent(ComponentType::ShipModule));
		if(module->isOwned())
		{
			AnomalyAcceleratorModule* anomalyAccelerator = static_cast<
				AnomalyAcceleratorModule*>(p_entities[i]->getComponent(
				ComponentType::AnomalyAcceleratorModule));
			anomalyAccelerator->cooldown -= dt;
			if(anomalyAccelerator->cooldown <= 0.0f && module->getActive())
			{
				anomalyAccelerator->cooldown = 1.0f;
				Transform* transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(p_entities[i],
					ComponentType::getTypeFor(ComponentType::Transform)));
				PhysicsBody* physBody = static_cast<PhysicsBody*>(
					p_entities[i]->getComponent(ComponentType::PhysicsBody));
				PhysicsSystem* physics = static_cast<PhysicsSystem*>(
					m_world->getSystem(SystemType::PhysicsSystem));
				AglVector3 moduleVelocity = physics->getController()->getBody(
					physBody->m_id)->GetVelocity();
				
			}
		}
	}
}