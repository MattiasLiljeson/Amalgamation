#pragma once

#include "EntitySystem.h"
#include "EntityWorld.h"
#include "Manager.h"
#include "SystemType.h"

class EntitySystem;
class EntityWorld;
class Manager;
class SystemType;

class SystemManager : public Manager
{
public:
	SystemManager( EntityWorld* p_world );
	~SystemManager();

	void initialize(){ initializeAll(); };
	EntitySystem* getSystem( SystemType::SystemTypeIdx p_systemIndex );
	void setSystem( EntitySystem* p_system, SystemType::SystemTypeIdx p_index );
	void initializeAll();
	void updateSynchronous();

private:
	map<SystemType::SystemTypeIdx, EntitySystem*> m_systems;
};

