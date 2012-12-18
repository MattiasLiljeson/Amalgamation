#include "ImpulseSystem.h"


ImpulseSystem::ImpulseSystem()
	: EntitySystem( SystemType::ImpulseSystem, 2, ComponentType::Impulse,
	ComponentType::PhysicsBody)
{
	//m_physicsController = something;
}

ImpulseSystem::~ImpulseSystem()
{

}

void ImpulseSystem::initialize()
{

}

void ImpulseSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		PhysicsBody* body =
			static_cast<PhysicsBody*>(
			m_world->getComponentManager()->getComponent( p_entities[i],
			ComponentType::getTypeFor(ComponentType::PhysicsBody)));

		Impulse* impulse =
			static_cast<Impulse*>(
			m_world->getComponentManager()->getComponent( p_entities[i],
			ComponentType::getTypeFor(ComponentType::Impulse)));

		if (body->m_id != -1)
		{
			m_physicsController->ApplyExternalImpulse(body->m_id, 
								impulse->getImpulse(), impulse->getAngularImpulse());
			m_world->getComponentManager()->removeComponent(
				p_entities[i], ComponentType::Impulse );
		}
	}
}

void ImpulseSystem::initializeEntity( Entity* p_entity )
{

}
