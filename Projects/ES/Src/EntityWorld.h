#pragma once

#include "ComponentManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <vector>

using namespace std;

class ComponentManager;
class Entity;
class EntityManager;
class SystemManager;

class EntityWorld
{
public:
	EntityWorld();
	~EntityWorld();

	void initialize();

	void process();
protected:

public:
	float m_delta;

private:
	EntityManager* m_entityManager;
	ComponentManager* m_componentManager;
	SystemManager* m_systemManager;

	vector<Entity> m_added;	
	vector<Entity> m_changed;
	vector<Entity> m_deleted;
	vector<Entity> m_enable;
	vector<Entity> m_disable;

};

