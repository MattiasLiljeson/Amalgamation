#include "PhysicsSystem.h"
#include "Transform.h"

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
		}
	}
}

void PhysicsSystem::initializeEntity(Entity* p_entity)
{
	//TEMP: Initializes a box for each entity with physics
	Transform* t =
		static_cast<Transform*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::Transform)));

	PhysicsBody* body =
		static_cast<PhysicsBody*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::PhysicsBody)));

	static int shit = 0;
	AglVector3 v(1, 0, 0);
	if (shit > 0)
		 v = AglVector3(-1, 0, 0);
	body->m_id = m_physicsController->AddBox(t->getTranslation(), t->getScale()*2, 1, v, AglVector3(0, 0, 0), false);
	shit++;
	//TODO: Add support for general creation
}