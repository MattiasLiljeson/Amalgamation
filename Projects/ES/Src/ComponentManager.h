#pragma once

#include "Component.h"
#include "ComponentType.h"
#include "Entity.h"
#include "Manager.h"
#include <bitset>
#include <vector>

using namespace std;

class Component;
class ComponentType;
class Entity;
class Manager;

class ComponentManager : public Manager
{
public:
	ComponentManager();
	~ComponentManager();

	void initialize(){};
	void deleted( Entity* p_entity );
	vector<Component*>& getComponentsFor( Entity* p_entity, vector<Component*>& p_fillBag );
	void clean();
	void addComponent( Entity* p_entity, ComponentType p_type, Component* p_component );
	Component* getComponent( Entity* p_entity, ComponentType p_type );

protected:
	void removeComponent( Entity* p_entity, ComponentType p_type );
	vector<Component*> getComponentsByType( ComponentType p_type );

private:
	void removeComponentsOfEntity( Entity* p_entity );

private:
	vector< vector<Component*> > m_componentsByType;
	vector<Entity*> m_deleted;
};

