#include "ModuleHighlightSystem.h"
#include "ShipHighlight.h"
#include "GradientComponent.h"
#include "Transform.h"
#include "ShipModule.h"

ModuleHighlightSystem::ModuleHighlightSystem()
	: EntitySystem(SystemType::ModuleHighlightSystem, 1,
	ComponentType::ShipModule)
{
}

void ModuleHighlightSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{

	}
}