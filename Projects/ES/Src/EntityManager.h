#pragma once

#include "Entity.h"
#include "EntityWorld.h"
#include "Manager.h"
#include <stack>
#include <vector>

using namespace std;

class Entity;
class EntityWorld;
class Manager;

class EntityManager : public Manager
{
public:
	EntityManager();
	~EntityManager();

	void initialize(){};
	Entity* createEntityInstance();
	void added( Entity* p_entity );
	void enabled( Entity* p_entity );
	void disabled( Entity* p_entity );
	void deleted( Entity* p_entity );
	bool isActive( int entityId ) const;
	bool isEnabled( int entityId ) const;
	Entity* getEntity( int entityid ) const;
	int getActiveEntityCount() const;
	int getTotalCreated() const;
	int getTotalAdded() const;
	int getTotalDeleted() const;
	Entity* getFirstEntityByComponentType( ComponentType::ComponentTypeIdx p_componentType );

private:
	// Use deque instead of vector maybe? 
	// Profile and measure!
	vector<Entity*> m_entities;
	vector<bool> m_disabled;
	stack<int> m_availableIds;

	int m_nextAvailableId;
	int m_active;
	int m_added;
	int m_created;
	int m_deleted;
};

