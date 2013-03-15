#include "RocketControllerSystem.h"
#include "ShipManagerSystem.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include <PhysicsController.h>
#include "SpawnSoundEffectPacket.h"
#include "ShipModule.h"
#include "ParticleSystemCreationInfo.h"
#include "SpawnExplosionPacket.h"
#include "MeshOffsetTransform.h"
#include "PlayerComponent.h"
#include "ModuleHelper.h"
#include "ScoreRuleHelper.h"

RocketControllerSystem::RocketControllerSystem(TcpServer* p_server)
	: EntitySystem(SystemType::RocketControllerSystem, 3, ComponentType::StandardRocket,
	ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_turnPower = 8.0f;
	m_enginePower = 150.0f;
	m_server = p_server;
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
	float waitUntilActivation = 0.5f;
	float rocketMaxAge = 15.0f;
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		StandardRocket* rocket = static_cast<StandardRocket*>(p_entities[i]->getComponent(ComponentType::StandardRocket));

		if (rocket->m_age == 0)
		{
			//Align with movement direction
			MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
				( p_entities[i]->getComponent( ComponentType::MeshOffsetTransform) );

			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
			AglMatrix world = body->GetWorld();

			world = meshOffset->offset.inverse()*world;
			world = pb->getOffset()*world;
			body->setTransform(world);
		}

		rocket->m_age += dt;
		//Check collision
		if (rocket->m_age > waitUntilActivation && rocket->m_age <= rocketMaxAge)
		{
			//Start targeting ships
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));

			if (rocket->m_target >= 0)
			{
				ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

				Entity* ship = m_world->getEntity(rocket->m_target);

				Transform* from = static_cast<Transform*>(p_entities[i]->getComponent(ComponentType::Transform));
				Transform* to = static_cast<Transform*>(ship->getComponent(ComponentType::Transform));

				//START APPLY IMPULSE

				MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
					( p_entities[i]->getComponent( ComponentType::MeshOffsetTransform) );

				AglVector3 imp = to->getTranslation() - from->getTranslation();
				float distance = imp.length();
				imp.normalize();

				AglMatrix world = meshOffset->offset * pb->getOffset().inverse() * body->GetWorld();

				AglVector3 rotAxis = AglVector3::crossProduct(imp, -world.GetForward());
				rotAxis.normalize();

				//Compute fraction of turn power that should be applied
				AglVector3 angVel = body->GetAngularVelocity();

				float angVelAxis = max(AglVector3::dotProduct(angVel, rotAxis), 0);

				float amountToRotate = min(1.0f - AglVector3::dotProduct(imp, world.GetForward()), 1.0f); //0 -> 1

				float frac = 1.0f;
				if (amountToRotate / angVelAxis < 0.25f)
				{
					frac = (amountToRotate / angVelAxis) / 0.25f;
				}

				rotAxis *= m_turnPower * dt * frac;

				frac = 1-frac;

				AglVector3 impulse = imp*0.25f + world.GetForward();
				impulse.normalize();
				impulse *= dt*m_enginePower*frac;

				ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);
			}
			else
			{
				MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>
					( p_entities[i]->getComponent( ComponentType::MeshOffsetTransform) );

				AglMatrix world = meshOffset->offset * pb->getOffset().inverse() * body->GetWorld();
				AglVector3 impulse = world.GetForward() * m_enginePower * dt;
				ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, AglVector3(0, 0, 0));
			}


			//END APPLY IMPULSE

			//Check collision	
			vector<unsigned int> cols = ps->getController()->CollidesWith(pb->m_id);
			bool staticCol = ps->getController()->CollidesWithStaticEnvironment(pb->m_id);
			if (cols.size() > 0 || staticCol)
			{
				explodeRocket(ps, pb, body, p_entities[i]);
			}
		}// if (rocket->m_age > waitUntilActivation && rocket->m_age <= rocketMaxAge)
		else if(rocket->m_age > rocketMaxAge)
		{
			PhysicsBody* pb = static_cast<PhysicsBody*>(p_entities[i]->getComponent(ComponentType::PhysicsBody));
			PhysicsSystem* ps = static_cast<PhysicsSystem*>(m_world->getSystem(SystemType::PhysicsSystem));
			RigidBody* body = static_cast<RigidBody*>(ps->getController()->getBody(pb->m_id));
			explodeRocket(ps, pb, body, p_entities[i]);
		}
	}
}

void RocketControllerSystem::explodeRocket(PhysicsSystem* p_physicsSystem,
	PhysicsBody* p_physicsBody, RigidBody* p_rigidBody, Entity* p_entity)
{
	StandardRocket* rocket = static_cast<StandardRocket*>(p_entity->getComponent(ComponentType::StandardRocket));

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
					m_server->broadcastPacket(explosion.pack());
				}
				colModule->addDamageThisTick(damage, rocket->m_ownerId);

				//Give the attacker some score
				PlayerComponent* scoreComponent;
				ModuleHelper::FindScoreComponent(m_world, rocket->m_ownerId, &scoreComponent);
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
	m_server->broadcastPacket(explosion.pack());

	m_world->deleteEntity(p_entity);
}


//Old funny fish
//Start targeting ships
/*ShipManagerSystem* shipManager = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

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

ps->getController()->ApplyExternalImpulse(pb->m_id, impulse, rotAxis);*/