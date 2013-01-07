#include "PhysicsSystem.h"
#include "Transform.h"
#include "BodyInitData.h"
#include <PhysicsController.h>
#include "PhysicsBody.h"

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
			// Get rigidbody from id
			Body* b = m_physicsController->getBody(body->m_id);

			// Handle parenting
			handleCompoundBodyDependencies(body,b);

			// Update the rigidbody
			AglMatrix world = b->GetWorld();
			Transform* t = static_cast<Transform*>( p_entities[i]->getComponent(
				ComponentType::Transform));
			AglVector3 pos;
			AglVector3 scale;
			AglQuaternion rot;
			AglMatrix::matrixToComponents(world, scale, rot, pos);
			t->setTranslation(pos);
			t->setRotation(rot);
			t->setScale(scale);
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
		CompoundBody* cb = NULL;
		int t=0;
		int* bodyId = &t; // temp storage of id
		AglVector3 offset=AglVector3(0.0f,0.0f,0.0f);
		if (init->m_compound)
		{
			// Add compound body as id to component
			body->m_id = m_physicsController->AddCompoundBody(init->m_position);
			cb = static_cast<CompoundBody*>(m_physicsController->getBody(body->m_id));
			offset = init->m_position;
		}
		else // repoint id storage; only add shape id to body component if not compound
			bodyId = &(body->m_id); 
		
		// Add shape
		if (init->m_type == 0)
		{
			*bodyId = m_physicsController->AddBox(init->m_position-offset,
				init->m_scale*2, 1, 
				init->m_velocity, 
				init->m_angularVelocity, 
				init->m_static,
				cb);
		}
		else
		{
			//Not Supported
		}
		
		// remove settings component
		m_world->getComponentManager()->removeComponent(
			p_entity, ComponentType::BodyInitData);
	}
	else // fallback settings
	{
		Transform* t =
			static_cast<Transform*>(
			m_world->getComponentManager()->getComponent( p_entity,
			ComponentType::getTypeFor(ComponentType::Transform)));
		body->m_id = m_physicsController->AddBox(t->getTranslation(), t->getScale()*2, 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
	}
}

void PhysicsSystem::handleCompoundBodyDependencies( PhysicsBody* p_bodyComponent, 
												   Body* p_rigidBody )
{
	if (p_bodyComponent->isParentChanged())
	{
		// First, retrieve the ids
		int oldId = p_bodyComponent->getOldParentId();
		int newId = p_bodyComponent->getParentId();

		// Then the pointers to the bodies
		Body* oldparent = NULL;
		Body* newparent = NULL;
		RigidBody* thisBody = static_cast<RigidBody*>(p_rigidBody);

		if (oldId!=-1)
			oldparent = m_physicsController->getBody(p_bodyComponent->getOldParentId());

		if (newId!=-1)
			newparent = m_physicsController->getBody(p_bodyComponent->getParentId());

		// Detach this body from old parent
		if (oldparent!=NULL && oldparent->IsCompoundBody())
		{
			static_cast<CompoundBody*>(oldparent)->DetachChild(thisBody);
		}

		// Attach this body to new parent
		if (newparent!=NULL && newparent->IsCompoundBody())
		{
			static_cast<CompoundBody*>(newparent)->AddChild(thisBody);
		}

		// Reset dirtybit
		p_bodyComponent->resetParentChangedStatus();
	}
}
