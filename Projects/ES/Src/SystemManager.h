#pragma once

#include "EntitySystem.h"
#include "EntityWorld.h"
#include "SystemType.h"

class EntitySystem;
class EntityWorld;
class SystemType;

class SystemManager
{
private:
	EntityWorld* m_world;
	map<SystemType::SystemTypeIdx, EntitySystem*> m_systems;

public:
	SystemManager( EntityWorld* p_world );
	~SystemManager();

	EntitySystem* getSystem( SystemType::SystemTypeIdx p_systemIndex );
	void setSystem( EntitySystem* p_system, SystemType::SystemTypeIdx p_index );
	void initializeAll();
	void updateSynchronous();
};

