#include "ShieldModuleControllerSystem.h"
#include "Transform.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include <Control.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"
#include "ShipModule.h"
#include "EntityCreationPacket.h"
#include "NetworkSynced.h"
#include "ShipModulesTrackerSystem.h"
#include <Globals.h>
#include "SpawnSoundEffectPacket.h"
#include "ShieldModuleActivation.h"
#include "AnimationUpdatePacket.h"
#include "ServerDynamicPhysicalObjectsSystem.h"
#include <PhysicsController.h>
#include "ModuleHelper.h"
#include "MeshOffsetTransform.h"
#include "ShieldActivationPacket.h"

ShieldModuleControllerSystem::ShieldModuleControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::ShieldModuleControllerSystem, 5,
	ComponentType::ShieldModule, ComponentType::ShipModule, ComponentType::Transform,
	ComponentType::PhysicsBody, ComponentType::MeshOffsetTransform)
{
	m_server = p_server;
}


ShieldModuleControllerSystem::~ShieldModuleControllerSystem()
{
}

void ShieldModuleControllerSystem::initialize()
{
	m_dynamicSystem = static_cast<ServerDynamicPhysicalObjectsSystem*>(m_world->getSystem(
		SystemType::ServerDynamicPhysicalObjectsSystem));
	m_physSystem = static_cast<PhysicsSystem*>(m_world->getSystem(
		SystemType::PhysicsSystem));
}

void ShieldModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			p_entities[i]->getComponent(ComponentType::ShipModule));
		if(module->isOwned())
		{
			ShieldModule* shield = static_cast<ShieldModule*>(
				p_entities[i]->getComponent(ComponentType::ShieldModule));
			shield->cooldown -= m_world->getDelta();
			shield->activation -= m_world->getDelta();
			if(shield->cooldown <= 0.0f && module->getActive())
			{
				shield->cooldown = shield->cooldownTime;
				shield->activation = shield->activationTime;
			}
			if(shield->activation > 0.0f && module->getActive())
			{
				if(!shield->toggleActive)
				{
					shield->toggleActive = true;
					ShieldActivationPacket data;
					data.shieldActivationState = true;
					data.entityIndex = p_entities[i]->getIndex();
					m_server->broadcastPacket(data.pack());

					//Play the push animation
					AnimationUpdatePacket anim;
					anim.networkIdentity = p_entities[i]->getIndex();
					anim.shouldPlay = true;
					anim.take = "Start";
					m_server->broadcastPacket(anim.pack());

					//Queue Active animation
					anim.shouldQueue = true;
					anim.take = "Active";
					m_server->broadcastPacket(anim.pack());

					//Queue Stop animation
					anim.take = "Stop";
					m_server->broadcastPacket(anim.pack());

					//Queue Default animation
					anim.take = "Default";
					m_server->broadcastPacket(anim.pack());

				}
				pushEntitiesBack(p_entities[i], m_dynamicSystem->getActiveEntities());
			}
			else
			{
				if(shield->toggleActive)
				{
					shield->toggleActive = false;
					ShieldActivationPacket data;
					data.shieldActivationState = false;
					data.entityIndex = p_entities[i]->getIndex();
					m_server->broadcastPacket(data.pack());
				}
			}
		}
	}
}

void ShieldModuleControllerSystem::pushEntitiesBack(Entity* p_shield,
	const vector<Entity*>& p_targets )
{
	Transform* shieldTransform = static_cast<Transform*>(p_shield->getComponent(
		ComponentType::Transform));
	ShieldModule* shieldModule = static_cast<ShieldModule*>(
		p_shield->getComponent(ComponentType::ShieldModule));
	for(unsigned int i=0; i<p_targets.size(); i++)
	{
		if(canTarget(p_shield, p_targets[i]))
		{
			Transform* targetTransform = static_cast<Transform*>(
				p_targets[i]->getComponent(ComponentType::Transform));
			AglVector3 dir = targetTransform->getTranslation() -
				shieldTransform->getTranslation();
			float lengthSquared = dir.lengthSquared();
			if(lengthSquared < shieldModule->maxRange * shieldModule->maxRange &&
				lengthSquared > shieldModule->minRange * shieldModule->minRange)
			{
				dir.normalize();
				AglVector3 shieldDir = getShieldDir(p_shield);
				if(AglVector3::dotProduct(dir, shieldDir) > 0.0f) // if in front of.
				{
					float dt = m_world->getDelta();
					PhysicsBody* targetBody = static_cast<PhysicsBody*>(p_targets[i]->
						getComponent(ComponentType::PhysicsBody));
					float length = sqrtf(lengthSquared);
					float rangeFactor = 1.0f - (length - shieldModule->minRange) /
						(shieldModule->maxRange - shieldModule->minRange);
					m_physSystem->getController()->ApplyExternalImpulse(targetBody->m_id,
						dir * shieldModule->impulse * rangeFactor * dt,
						AglVector3::zero());

				}
			}
		}
	}
}

bool ShieldModuleControllerSystem::canTarget( Entity* p_shield, Entity* p_target ) const
{
	ShipModule* module = static_cast<ShipModule*>(p_shield->getComponent(
		ComponentType::ShipModule));
	Entity* shieldOwner = NULL;
	ModuleHelper::FindParentShip(m_world, &shieldOwner, module);
	
	if(p_target == p_shield ||
		p_target == shieldOwner)
	{
		return false;
	}
	else
	{
		ShipModule* targetModule = static_cast<ShipModule*>(p_target->getComponent(
			ComponentType::ShipModule));
		if(targetModule)
		{
			Entity* targetOwner = NULL;
			ModuleHelper::FindParentShip(m_world, &targetOwner, targetModule);
			if(targetOwner &&
				targetOwner == shieldOwner)
			{
				return false;
			}
		}
	}
	return true;
}

AglVector3 ShieldModuleControllerSystem::getShieldDir( Entity* p_shield ) const
{
	AglVector3 dir = AglVector3::forward();
	Transform* transform = static_cast<Transform*>(
		p_shield->getComponent(ComponentType::Transform));
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(
		p_shield->getComponent(ComponentType::MeshOffsetTransform));
	dir.transform(offset->offset.inverse());
	transform->getRotation().transformVector(dir);
	return dir;
}