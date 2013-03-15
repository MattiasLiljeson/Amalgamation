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
#include "SpawnExplosionPacket.h"
#include "PlayerComponent.h"
#include "ModuleHelper.h"
#include "ScoreRuleHelper.h"

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
				RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
				explodeMine(ps, pb, body, p_entities[i]);
			}
		}	
	}
}
void MineControllerSystem::explodeMine(PhysicsSystem* p_physicsSystem,
										   PhysicsBody* p_physicsBody, RigidBody* p_rigidBody, Entity* p_entity)
{
	StandardMine* mine = static_cast<StandardMine*>(p_entity->getComponent(ComponentType::StandardMine));

	// Remove the rocket...
	p_physicsSystem->getController()->ApplyExternalImpulse(p_rigidBody->GetWorld().GetTranslation(), 20, 20);
	p_physicsSystem->getController()->InactivateBody(p_physicsBody->m_id);


	vector<pair<unsigned int, float>> collided = p_physicsSystem->getController()->GetObjectsWithinSphere(p_rigidBody->GetWorld().GetTranslation(), 20);
	for (unsigned int i = 0; i < collided.size(); i++)
	{
		Entity* colEn = p_physicsSystem->getEntity(collided[i].first);
		if(colEn)
		{
			ShipModule* colModule = static_cast<ShipModule*>(colEn->getComponent(ComponentType::ShipModule));

			if (colModule && colModule->m_parentEntity >= 0)
			{
				float damage = min(100, 1000 / collided[i].second);
				if (damage > colModule->m_health)
				{
					Transform* t = static_cast<Transform*>(colEn->getComponent(ComponentType::Transform));
					SpawnExplosionPacket explosion;
					explosion.position = t->getTranslation();
					explosion.source = ExplosionSource::MINE;
					m_server->broadcastPacket(explosion.pack());
				}
				colModule->addDamageThisTick(damage, mine->m_ownerId);

				//Give the attacker some score
				PlayerComponent* scoreComponent;
				ModuleHelper::FindScoreComponent(m_world, mine->m_ownerId, &scoreComponent);
				if (scoreComponent)
				{
					scoreComponent->addRelativeDamageScore(ScoreRuleHelper::scoreFromDamagingOpponent(damage));
				}
			}
		}
	}


	Transform* t = static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform));


	SpawnExplosionPacket explosion;
	explosion.position = t->getTranslation();
	explosion.source = ExplosionSource::MINE;
	m_server->broadcastPacket(explosion.pack());

	m_world->deleteEntity(p_entity);
}