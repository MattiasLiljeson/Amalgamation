#include <gtest/gtest.h>
#include "EntityWorld.h"
#include "InputSystem.h"

TEST(InputSystem, MouseButton)
{
	// Doesn't work. No messages are put in the message queue from the message loop as the 
	// message loop doesn't exist. XINPUT only works with windowed applications.
	EntityWorld* world = new EntityWorld();
	EntitySystem* sys = new InputSystem();
	world->setSystem( SystemType::InputSystem, sys, true);
	
	Entity* e = world->createEntity();
	e->addComponent( ComponentType::Input, new Input());
	world->addEntity(e);
	world->initialize();
	
	
	float dt = 1/60.0f;
	// Iterate 1000000 times
	for(int i=0; i<10000; i++)
	{
		world->setDelta(dt);
		world->process();
	}

}