#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
	: EntitySystem(SystemType::PhysicsSystem, 1, /*ComponentType::PhysUnknown,*/ ComponentType::PhysicsBody)
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
			m_world->getComponentManager()->getComponent( p_entities[0],
			ComponentType::getTypeFor(ComponentType::PhysicsBody)));
	}
}