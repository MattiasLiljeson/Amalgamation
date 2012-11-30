#include <gtest/gtest.h>
#include "EntitySystem.h"

TEST(EntitySystemConstructor, SimpleCreation)
{
	SystemType type = SystemType::getTypeFor(SystemType::NON_EXISTING);
	EntitySystem system( type, 1, ComponentType::Position);
}