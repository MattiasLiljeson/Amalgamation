#include "ServerStaticObjectsSystem.h"

ServerStaticObjectsSystem::ServerStaticObjectsSystem() 
	: EntitySystem(SystemType::ServerStaticObjectsSystem, 1, ComponentType::StaticProp)
{

}

ServerStaticObjectsSystem::~ServerStaticObjectsSystem()
{

}

void ServerStaticObjectsSystem::processEntities(const vector<Entity*>& p_entities )
{
}

void ServerStaticObjectsSystem::initialize()
{
}

//void ServerStaticObjectsSystem::added( Entity* p_entity )
//{
//	if(p_entity->getComponent(ComponentType::StaticProp) != NULL)
//		m_objects.push_back(p_entity);
//}

vector<Entity*> ServerStaticObjectsSystem::getStaticObjects()
{
	return getActiveEntities();
	//return m_objects;
}
