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

	/// Fetch Component
	Component* getComponent( Entity* p_entity, ComponentType p_type );
	Component* getComponent( Entity* p_entity, ComponentType::ComponentTypeIdx p_typeIdx );
	Component* getComponent( int p_entityIdx, ComponentType p_type );
	Component* getComponent( int p_entityIdx, ComponentType::ComponentTypeIdx p_typeIdx );

	void removeComponent( Entity* p_entity, ComponentType p_type );
	void removeComponent( Entity* p_entity, ComponentType::ComponentTypeIdx p_typeIdx );

	void postPerform();
private:
	vector<Component*> getComponentsByType( ComponentType p_type );
	void removeComponentsOfEntity( Entity* p_entity );

private:
	vector< vector<Component*> > m_componentsByType;
	vector<Entity*> m_deleted;

	vector<Component*> m_deleteOnPost;
	vector< pair<int, int> > m_deleteComponentsByTypeOnPostPerform;
	// m_deleteComponentsByTypeOnPostPerform.push_back(
	//		&m_componentsByType[p_typeIdx][entityIndex] );
};

