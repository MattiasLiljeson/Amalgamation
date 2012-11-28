#include <gtest/gtest.h>
#include "EntitySystem.h"

TEST(EntitySystemConstructor, SimpleCreation)
{
	EntitySystem system(1, ComponentType::Position);

}