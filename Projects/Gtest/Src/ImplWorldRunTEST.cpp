#include <gtest/gtest.h>
#include "EntityWorld.h"
#include "EntitySystem.h"

TEST(RunningWorld, SimpleWorldScenario)
{
	EntityWorld world;

	SystemType type = SystemType::getTypeFor(SystemType::EntitySystem);

	world.setSystem(SystemType::EntitySystem, new EntitySystem(type), true);

}