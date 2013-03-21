#include "ModuleCleaningService.h"
#include "ShipModule.h"
#include "ModuleOnChamberStartPoint.h"
#include "GameplayTags.h"

ModuleCleaningService::ModuleCleaningService()
	: EntitySystem(SystemType::ModuleCleaningService, 1, ComponentType::ShipModule)
{
}

ModuleCleaningService::~ModuleCleaningService()
{
}

void ModuleCleaningService::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(p_entities[i]->getComponent(
			ComponentType::ShipModule));
		ModuleDragging_TAG* tag = static_cast<ModuleDragging_TAG*>(p_entities[i]->
			getComponent(ComponentType::TAG_ModuleDragging));
		ModuleOnChamberStartPoint* onSpawn = static_cast<ModuleOnChamberStartPoint*>(
			p_entities[i]->getComponent(ComponentType::ModuleOnChamberSpawnPoint));
		if(onSpawn == NULL && tag == NULL && module->m_parentEntity < 0)
		{
			module->m_lifetime -= m_world->getDelta();
			if(module->m_lifetime <= 0.0f)
			{
				m_world->deleteEntity(p_entities[i]);
			}
		}
	}
}

void ModuleCleaningService::inserted( Entity* p_entity )
{

}