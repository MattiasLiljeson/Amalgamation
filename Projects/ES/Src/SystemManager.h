#pragma once

#include "EntityWorld.h"

class EntitySystem;
class EntityWorld;

class SystemManager
{
private:
	EntityWorld* m_world;
	map<type_info, EntitySystem*> m_systems;

public:
	SystemManager( EntityWorld* p_world );
	~SystemManager();

	EntitySystem* getSystem( type_info p_system );
	void setSystem( EntitySystem* p_system );
	void initializeAll();
	void updateSynchronous();
};

