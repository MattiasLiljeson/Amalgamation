#include <gtest/gtest.h>
#include <bitset>

#include "EntitySystem.h"

using namespace std;

TEST(EntitySystemConstructor, SimpleCreation)
{
	EntitySystem system(1, ComponentType::Position);
}

TEST(EntitySystemConstructor, MultipleComponentTypes)
{
	EntitySystem system(3,
		ComponentType::Position,
		ComponentType::Orientation,
		ComponentType::Render);

	bitset<ComponentType::NUM_COMPONENT_TYPES> set;
	set = system.getComponentBits();

	EXPECT_EQ(bitset<ComponentType::NUM_COMPONENT_TYPES>("0111"), set);
}