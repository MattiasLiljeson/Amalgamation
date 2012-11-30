#include <gtest/gtest.h>
#include <bitset>

#include "EntitySystem.h"

using namespace std;

TEST(EntitySystemConstructor, SimpleCreation)
{
	SystemType type = SystemType::getTypeFor(SystemType::NON_EXISTING);
	EntitySystem system( type, 1, ComponentType::Position);
}

TEST(EntitySystemConstructor, MultipleComponentTypes)
{
	SystemType type = SystemType::getTypeFor(SystemType::NON_EXISTING);

	EntitySystem system(type, 3,
		ComponentType::Position,
		ComponentType::Orientation,
		ComponentType::Render);

	bitset<ComponentType::NUM_COMPONENT_TYPES> set;
	set = system.getComponentBits();

	EXPECT_EQ(bitset<ComponentType::NUM_COMPONENT_TYPES>("0111"), set);
}