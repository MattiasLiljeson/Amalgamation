#include "AnomalyBombControllerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "AnomalyBomb.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>

AnomalyBombControllerSystem::AnomalyBombControllerSystem()
	: EntitySystem(SystemType::AnomalyBombControllerSystem, 4, ComponentType::AnomalyBomb,
	ComponentType::Transform, ComponentType::PhysicsBody, ComponentType::NetworkSynced)
{
}

void AnomalyBombControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	const vector<Entity*>& dynamicEntities = m_world->getSystem(
		SystemType::ServerDynamicPhysicalObjectsSystem)->getActiveEntities();
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		AnomalyBomb* bombBomb = static_cast<AnomalyBomb*>(p_entities[i]->getComponent(
			ComponentType::AnomalyBomb));
		Transform* bombTransform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));
		bombBomb->lifeTime -= dt;
		if(bombBomb->lifeTime <= 0.0f)
		{
			m_world->deleteEntity(p_entities[i]);
		}
		else if(bombBomb->lifeTime <= bombBomb->explodeTime)
		{
			for(unsigned int netsyncIndex=0; netsyncIndex<dynamicEntities.size(); netsyncIndex++)
			{
				Transform* otherTransform = static_cast<Transform*>(
					dynamicEntities[netsyncIndex]->getComponent(ComponentType::Transform));
				AglVector3 dir = ( bombTransform->getTranslation() -
					otherTransform->getTranslation() );
				float lengthSquared = dir.lengthSquared();
				if(lengthSquared <= bombBomb->eventHorizonRadius * bombBomb->eventHorizonRadius)
				{
					// Stillness.
				}
				else if(lengthSquared <= bombBomb->arriveRadius * bombBomb->arriveRadius)
				{
					PhysicsBody* body = static_cast<PhysicsBody*>(
						dynamicEntities[netsyncIndex]->getComponent(
						ComponentType::PhysicsBody));
					vector<ComponentType::ComponentTypeIdx> bombComps =
						m_world->getComponentManager()->getComponentEnumList(p_entities[i]);
					vector<ComponentType::ComponentTypeIdx> otherComps =
						m_world->getComponentManager()->getComponentEnumList(
						dynamicEntities[netsyncIndex]);
					PhysicsSystem* physSystem = static_cast<PhysicsSystem*>(
						m_world->getSystem(SystemType::PhysicsSystem));
					float length = dir.length();
					dir.normalize();
					float radiusFactor = (length - bombBomb->eventHorizonRadius) /
						(bombBomb->arriveRadius - bombBomb->eventHorizonRadius);
					physSystem->getController()->ApplyExternalImpulse(
						body->m_id, dir * radiusFactor * dt * bombBomb->impulse,
						AglVector3::zero());
				}
				else if(lengthSquared <= bombBomb->radius * bombBomb->radius)
				{
					PhysicsBody* body = static_cast<PhysicsBody*>(
						dynamicEntities[netsyncIndex]->getComponent(
						ComponentType::PhysicsBody));
					vector<ComponentType::ComponentTypeIdx> bombComps =
						m_world->getComponentManager()->getComponentEnumList(p_entities[i]);
					vector<ComponentType::ComponentTypeIdx> otherComps =
						m_world->getComponentManager()->getComponentEnumList(
						dynamicEntities[netsyncIndex]);
					PhysicsSystem* physSystem = static_cast<PhysicsSystem*>(
						m_world->getSystem(SystemType::PhysicsSystem));
					float length = dir.length();
					dir.normalize();
					float radiusFactor = (length - bombBomb->arriveRadius) /
						(bombBomb->radius - bombBomb->arriveRadius);
					physSystem->getController()->ApplyExternalImpulse(
						body->m_id, dir * (1.0f - radiusFactor) * dt * bombBomb->impulse,
						AglVector3::zero());
					
				}
			}
		}
	}
}