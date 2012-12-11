#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
	: EntitySystem(SystemType::PhysicsSystem, 1, ComponentType::PhysUnknown)
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::initialize()
{
}

void PhysicsSystem::processEntities(const vector<Entity*>& p_entities)
{
}