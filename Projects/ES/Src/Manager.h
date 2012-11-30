#pragma once

#include "Entity.h"
#include "EntityWorld.h"
#include "IEntityObserver.h"

class Entity;
class EntityWorld;
class IEntityObserver;

class Manager : public IEntityObserver
{
public:
	enum ManagerTypeIdx
	{
		NON_EXISTING_MANAGER,
		// Add types here
		ComponentManager,
		EntityManager,
		SystemManager,
		NUM_MANAGER_TYPES
	};

	virtual void initialize() = 0;
	virtual void added( Entity* p_entity ) {}
	virtual void changed( Entity* p_entity ) {}
	virtual void deleted( Entity* p_entity ) {}
	virtual void disabled( Entity* p_entity ) {}
	virtual void enabled( Entity* p_entity ) {}

	virtual void setWorld( EntityWorld* p_world ) { m_world = p_world; }
	virtual EntityWorld* getWorld() { return m_world; } 

protected:
	EntityWorld* m_world;
};