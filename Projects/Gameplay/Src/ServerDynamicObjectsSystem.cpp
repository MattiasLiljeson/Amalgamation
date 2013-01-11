#include "ServerDynamicObjectsSystem.h"

ServerDynamicObjectsSystem::ServerDynamicObjectsSystem() 
	: EntitySystem( SystemType::ServerDynamicObjectsSystem, 1,
	ComponentType::NetworkSynced)
{

}

ServerDynamicObjectsSystem::~ServerDynamicObjectsSystem()
{

}

void ServerDynamicObjectsSystem::processEntities(const vector<Entity*>& p_entities )
{
	int x = 0;
}

void ServerDynamicObjectsSystem::initialize()
{
}

void ServerDynamicObjectsSystem::added( Entity* p_entity )
{
	if(p_entity->getComponent(ComponentType::NetworkSynced) != NULL)
		m_objects.push_back(p_entity);
}

vector<Entity*> ServerDynamicObjectsSystem::getDynamicObjects()
{
	return m_objects;
}

void ServerDynamicObjectsSystem::deleted( Entity* p_entity )
{
	for (unsigned int i = 0; i < m_objects.size(); i++)
	{
		if(p_entity == m_objects[i])
		{
			m_objects.erase(m_objects.begin() + i);
			break;
		}
	}
}
