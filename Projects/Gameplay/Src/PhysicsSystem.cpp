#include "PhysicsSystem.h"
#include "Transform.h"
#include "BodyInitData.h"

PhysicsSystem::PhysicsSystem()
	: EntitySystem(SystemType::PhysicsSystem, 2, ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_physicsController = new PhysicsController();
}


PhysicsSystem::~PhysicsSystem()
{
	delete m_physicsController;
}

void PhysicsSystem::initialize()
{
}

void PhysicsSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();
	m_physicsController->Update(dt);

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		PhysicsBody* body =
			static_cast<PhysicsBody*>(
			m_world->getComponentManager()->getComponent( p_entities[i],
			ComponentType::getTypeFor(ComponentType::PhysicsBody)));

		if (body->m_id == -1)
		{
			//Not initialized! Initialize the physics component of the entity
			initializeEntity(p_entities[i]);
		}
		else
		{
			Body* b = m_physicsController->getBody(body->m_id);
			AglMatrix world = b->GetWorld();
			Transform* t = static_cast<Transform*>( p_entities[i]->getComponent(
				ComponentType::Transform));
			AglVector3 pos;
			AglVector3 scale;
			AglQuaternion rot;
			AglMatrix::matrixToComponents(world, scale, rot, pos);
			t->setTranslation(pos);
			t->setRotation(rot);
		}
	}
}

void PhysicsSystem::applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse)
{
	m_physicsController->ApplyExternalImpulse(p_bodyID, p_impulse, p_angularImpulse);
}

void PhysicsSystem::initializeEntity(Entity* p_entity)
{
	PhysicsBody* body =
		static_cast<PhysicsBody*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::PhysicsBody)));

	BodyInitData* init =
		static_cast<BodyInitData*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::BodyInitData)));

	if (init)
	{
		if (init->m_type == 0)
		{
			body->m_id = m_physicsController->AddBox(init->m_position, init->m_scale*2, 1, init->m_velocity, init->m_angularVelocity, init->m_static);
		}
		else
		{
			//Not Supported
		}
		m_world->getComponentManager()->removeComponent(
			p_entity, ComponentType::BodyInitData);
	}
	else
	{
		Transform* t =
			static_cast<Transform*>(
			m_world->getComponentManager()->getComponent( p_entity,
			ComponentType::getTypeFor(ComponentType::Transform)));
		body->m_id = m_physicsController->AddBox(t->getTranslation(), t->getScale()*2, 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
	}
}