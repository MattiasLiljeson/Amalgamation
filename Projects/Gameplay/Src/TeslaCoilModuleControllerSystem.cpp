#include "TeslaCoilModuleControllerSystem.h"
#include "ShipModule.h"
#include "TeslaCoilModule.h"
#include "ShipModulesTrackerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include <TcpClient.h>
#include "NetworkSynced.h"

TeslaCoilModuleControllerSystem::TeslaCoilModuleControllerSystem()
	: EntitySystem(SystemType::TeslaCoilModuleControllerSystem, 5,
	ComponentType::TeslaCoilModule, ComponentType::ShipModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::NetworkSynced)
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
				NetworkSynced* teslaNetsync = static_cast<NetworkSynced*>(p_entities[i]->
					getComponent(ComponentType::NetworkSynced));
				fireTeslaCoil(p_entities[i], teslaCoil, teslaTransform, teslaNetsync);
			}
		}
	}
}

void TeslaCoilModuleControllerSystem::fireTeslaCoil(Entity* p_teslaEntity,
	TeslaCoilModule* p_teslaModule, Transform* p_teslaTransform,
	NetworkSynced* p_teslaNetsync)
{
	ShipModulesTrackerSystem* moduleTracker = static_cast<
		ShipModulesTrackerSystem*>(m_world->getSystem(
		SystemType::ShipModulesTrackerSystem));
	AglVector3 teslaPosition = p_teslaTransform->getTranslation();
	for(unsigned int otherModuleIndex=0;
		otherModuleIndex<moduleTracker->getActiveEntities().size();
		otherModuleIndex++)
	{
		Entity* otherModule = moduleTracker->getActiveEntities()[otherModuleIndex];
		AglVector3 otherModulePosition = static_cast<Transform*>(otherModule->getComponent(
			ComponentType::Transform))->getTranslation();
		AglVector3 distanceVector = otherModulePosition - teslaPosition;
		if(distanceVector.lengthSquared() < p_teslaModule->range * p_teslaModule->range)
		{
			PhysicsBody* body = static_cast<PhysicsBody*>(
				otherModule->getComponent(ComponentType::PhysicsBody));
			ShipModule* otherShipModule = static_cast<ShipModule*>(
				otherModule->getComponent(ComponentType::ShipModule));
			float distance = distanceVector.length();
			float damageMultiplier = 1.0f;
			otherShipModule->addDamageThisTick(damageMultiplier * p_teslaModule->damage,
				p_teslaNetsync->getNetworkOwner());
		}
	}
}

float TeslaCoilModuleControllerSystem::calculateMultiplier(float p_distance,
	float p_optimalRange, float p_range)
{
	float damageMultiplier = 1.0f;
	if(p_distance > p_optimalRange)
	{
		damageMultiplier = 1.0f - (p_distance - p_optimalRange) /
			(p_range - p_optimalRange);
	}
	return damageMultiplier;
}