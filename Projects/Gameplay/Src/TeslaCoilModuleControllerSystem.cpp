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
#include <RandomUtil.h>
#include "TeslaHitPacket.h"
#include <TcpServer.h>
#include <algorithm>
#include "ModuleHelper.h"

TeslaCoilModuleControllerSystem::TeslaCoilModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::TeslaCoilModuleControllerSystem, 7,
	ComponentType::TeslaCoilModule, ComponentType::ShipModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::NetworkSynced,
	ComponentType::SpawnPointSet, ComponentType::MeshOffsetTransform)
{
	m_server = p_server;
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
				ShipModule* teslaShipModule = static_cast<ShipModule*>(p_entities[i]->
					getComponent(ComponentType::ShipModule));
				fireTeslaCoil(p_entities[i], teslaCoil, teslaTransform, teslaNetsync,
					teslaShipModule);
			}
		}
	}
}

void TeslaCoilModuleControllerSystem::fireTeslaCoil(Entity* p_teslaEntity,
	TeslaCoilModule* p_teslaModule, Transform* p_teslaTransform,
	NetworkSynced* p_teslaNetsync, ShipModule* p_teslaShipModule)
{
	vector<int> entitiesHit;
	ShipModulesTrackerSystem* moduleTracker = static_cast<
		ShipModulesTrackerSystem*>(m_world->getSystem(
		SystemType::ShipModulesTrackerSystem));
	AglVector3 teslaPosition = p_teslaTransform->getTranslation();
	for(unsigned int otherModuleIndex=0;
		otherModuleIndex<moduleTracker->getActiveEntities().size();
		otherModuleIndex++)
	{
		Entity* otherEntity = moduleTracker->getActiveEntities()[otherModuleIndex];
		ShipModule* otherShipModule = static_cast<ShipModule*>(otherEntity->getComponent(
			ComponentType::ShipModule));
		if(canTarget(p_teslaShipModule, otherShipModule))
		{
			AglVector3 otherModulePosition = static_cast<Transform*>(otherEntity->getComponent(
				ComponentType::Transform))->getTranslation();
			AglVector3 distanceVector = otherModulePosition - teslaPosition;
			if(distanceVector.lengthSquared() < p_teslaModule->range * p_teslaModule->range)
			{
				PhysicsBody* body = static_cast<PhysicsBody*>(
					otherEntity->getComponent(ComponentType::PhysicsBody));
				ShipModule* otherShipModule = static_cast<ShipModule*>(
					otherEntity->getComponent(ComponentType::ShipModule));
				float distance = distanceVector.length();
				float hitChance = calculateHitChance(distance, p_teslaModule->optimalRange,
					p_teslaModule->range);
				if(RandomUtil::randomSingle() <= hitChance)
				{
					if(otherShipModule->m_parentEntity != p_teslaShipModule->m_parentEntity)
					{
						otherShipModule->addDamageThisTick(hitChance * p_teslaModule->damage,
							p_teslaNetsync->getNetworkOwner());
						entitiesHit.push_back(otherEntity->getIndex());
					}
				}
			}
		}//if canTarget
	}//for: otherModuleIndex
	if(!entitiesHit.empty())
	{
		// A custom compare Functor
		struct LengthCompare
		{
			EntityWorld* world;
			AglVector3 sourcePosition;
			LengthCompare(EntityWorld* p_world, AglVector3 p_sourcePosition)
			{
				world = p_world;
				sourcePosition = p_sourcePosition;
			}
			bool operator() (int p_firstIndex, int p_secondIndex)
			{
				AglVector3 firstPosition = static_cast<Transform*>(
					world->getComponentManager()->getComponent(p_firstIndex,
					ComponentType::Transform))->getTranslation();
				AglVector3 secondPosition = static_cast<Transform*>(
					world->getComponentManager()->getComponent(p_secondIndex,
					ComponentType::Transform))->getTranslation();
				float firstLengthSquared = (firstPosition - sourcePosition).lengthSquared();
				float secondLengthSquared = (secondPosition - sourcePosition).lengthSquared();
				return firstLengthSquared < secondLengthSquared;
			}
		} myLengthCompare(m_world, static_cast<Transform*>(p_teslaEntity->getComponent(
			ComponentType::Transform))->getTranslation());
		//struct LightCompare

		std::sort(entitiesHit.begin(), entitiesHit.begin() + entitiesHit.size(), myLengthCompare);
		unsigned int i=0;
		TeslaHitPacket hitPacket;
		hitPacket.identitySource = p_teslaEntity->getIndex();
		while(i < (unsigned int)hitPacket.NUM_TESLA_HITS_MAX && i < entitiesHit.size())
		{
			hitPacket.identitiesHit[i] = entitiesHit[i];
			i++;
		}
		hitPacket.numberOfHits = static_cast<unsigned char>(i);
		m_server->broadcastPacket(hitPacket.pack());
	}
}

float TeslaCoilModuleControllerSystem::calculateHitChance(float p_distance,
	float p_optimalRange, float p_range)
{
	float hitChance = 1.0f;
	if(p_distance > p_optimalRange)
	{
		hitChance = 1.0f - (p_distance - p_optimalRange) /
			(p_range - p_optimalRange);
	}
	return hitChance;
}

bool TeslaCoilModuleControllerSystem::canTarget( ShipModule* p_teslaModule,
	ShipModule* p_otherModule ) const
{
	Entity* sourceParent = NULL;
	ModuleHelper::FindParentShip(m_world, &sourceParent, p_teslaModule);
	Entity* targetParent = NULL;
	ModuleHelper::FindParentShip(m_world, &targetParent, p_otherModule);
	bool target = false;
	if(sourceParent != NULL && targetParent != NULL)
	{
		target = sourceParent->getIndex() != targetParent->getIndex();
	}
	else if(targetParent == NULL)
	{
		target = true;
	}
	return target;
}