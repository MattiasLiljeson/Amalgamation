#include <gtest/gtest.h>

#include "EntityWorld.h"

TEST(EntityWorldBasic, EntityWorldInstancingTest)
{
	EntityWorld* world = new EntityWorld();


	EntitySystem* system = new EntitySystem(SystemType::NON_EXISTING, 3,
		ComponentType::Transform,
		ComponentType::Render,
		ComponentType::NetworkSynced);

	world->setSystem(SystemType::getTypeFor(SystemType::EntitySystem), system, true);
	Entity* ent = world->createEntity();

	float dt = 1.0f/60;

	// Iterate 10000 times
	for(int i=0; i<10000; i++)
	{
		world->setDelta(dt);
		world->process();
	}
}