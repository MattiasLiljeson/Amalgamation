#pragma once

//#include "EntityManager.h"
//#include "ComponentManager.h"
#include "SystemManager.h"
//#include "Entity.h"

using namespace std;

//class EntityManager;
//class ComponentManager;
class SystemManager;

class EntityWorld
{
public:
	EntityWorld(void);
	~EntityWorld(void);

	void process();
protected:

private:
	//ComponentManager*	componentManager;
	//EntityManager*		entityManager;

	SystemManager*		systemManager;
};

