#include "ShipModulesTrackerSystem.h"

ShipModulesTrackerSystem::ShipModulesTrackerSystem()
	: EntitySystem(SystemType::ShipModulesTrackerSystem, 4, ComponentType::ShipModule,
	ComponentType::Transform, ComponentType::PhysicsBody, ComponentType::NetworkSynced)
{
}

vector<ShipModule*> ShipModulesTrackerSystem::getModulesFromParent( int p_entityIndex )
{
	vector<ShipModule*> modules;
	const vector<Entity*>& entities = getActiveEntities();
	for(unsigned int i=0; i<entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			entities[i]->getComponent(ComponentType::ShipModule));
		if(module->m_parentEntity == p_entityIndex)
		{
			modules.push_back(module);
		}
	}
	return modules;
}

void ShipModulesTrackerSystem::destroyAllModules()
{
	const vector<Entity*>& entities = getActiveEntities();
	for(unsigned int i=0; i<entities.size(); i++)
	{
		m_world->deleteEntity(entities[i]);
	}
}
