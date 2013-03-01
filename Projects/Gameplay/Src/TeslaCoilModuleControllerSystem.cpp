#include "TeslaCoilModuleControllerSystem.h"
#include "ShipModule.h"
#include "TeslaCoilModule.h"
#include "ShipModulesTrackerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>

TeslaCoilModuleControllerSystem::TeslaCoilModuleControllerSystem()
	: EntitySystem(SystemType::TeslaCoilModuleControllerSystem, 4,
	ComponentType::TeslaCoilModule, ComponentType::ShipModule, ComponentType::Transform,
	ComponentType::PhysicsBody)
{
}

void TeslaCoilModuleControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(p_entities[i]->getComponent(
			ComponentType::ShipModule));
		if(module->isOwned())
		{
			TeslaCoilModule* teslaCoil = static_cast<TeslaCoilModule*>(
				p_entities[i]->getComponent(ComponentType::TeslaCoilModule));
			teslaCoil->cooldown -= dt;
			if(teslaCoil->cooldown <= 0.0f && module->getActive())
			{
				teslaCoil->cooldown = teslaCoil->cooldownTime;
				Transform* teslaTransform = static_cast<Transform*>(p_entities[i]->
					getComponent(ComponentType::Transform));
				fireTeslaCoil(p_entities[i], teslaCoil, teslaTransform);
			}
		}
	}
}

void TeslaCoilModuleControllerSystem::fireTeslaCoil(Entity* p_teslaEntity,
	TeslaCoilModule* p_teslaModule, Transform* p_teslaTransform)
{
	ShipModulesTrackerSystem* moduleTracker = static_cast<
		ShipModulesTrackerSystem*>(m_world->getSystem(
		SystemType::ShipModulesControllerSystem));
	AglVector3 teslaPosition = p_teslaTransform->getTranslation();
	for(unsigned int otherModuleIndex=0;
		otherModuleIndex<moduleTracker->getActiveEntities().size();
		otherModuleIndex++)
	{
		AglVector3 otherModulePosition = static_cast<Transform*>(
			moduleTracker->getActiveEntities()[otherModuleIndex]->
			getComponent(ComponentType::Transform))->getTranslation();
		AglVector3 distanceVector = otherModulePosition - teslaPosition;
		if(distanceVector.lengthSquared() < p_teslaModule->range * p_teslaModule->range)
		{
			PhysicsBody* body = static_cast<PhysicsBody*>(
				moduleTracker->getActiveEntities()[otherModuleIndex]->
				getComponent(ComponentType::PhysicsBody));
			if(p_teslaEntity != moduleTracker->getActiveEntities()[otherModuleIndex]
				&& body != NULL)
			{
				static_cast<PhysicsSystem*>(m_world->getSystem(
					SystemType::PhysicsSystem))->getController()->
					ApplyExternalImpulse(body->m_id, distanceVector,
					AglVector3::zero());
			}
		}
	}
}