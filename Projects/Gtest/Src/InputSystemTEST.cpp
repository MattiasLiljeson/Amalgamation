#include <gtest/gtest.h>
#include "EntityWorld.h"
#include "InputSystem.h"

TEST(InputSystem, MouseButton)
{
	EntityWorld* world = new EntityWorld();
	EntitySystem* sys = new InputSystem();
	world->setSystem( SystemType::InputSystem, sys, true);
	world->initialize();
	while(true)
		world->process();


}