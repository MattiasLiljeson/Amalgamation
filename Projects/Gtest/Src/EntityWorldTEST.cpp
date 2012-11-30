#include <gtest/gtest.h>

#include "EntityWorld.h"

TEST(EntityWorldBasic, EntityWorldInstancingTest)
{
	EntityWorld* world = new EntityWorld();

	SystemType type = SystemType::getTypeFor(SystemType::NON_EXISTING);

	EntitySystem* system = new EntitySystem(type, 3,
		ComponentType::Position,
		ComponentType::Orientation,
		ComponentType::Render);

	world->setSystem(SystemType::getTypeFor(SystemType::EntitySystem), system, true);
	Entity* ent = world->createEntity();

	float dt = 1.0f/60;

	while (true)
	{
		world->setDelta(dt);
		world->process();
	}
}