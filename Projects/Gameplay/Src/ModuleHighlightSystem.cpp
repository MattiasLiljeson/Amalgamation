#include "ModuleHighlightSystem.h"
#include "ShipHighlight.h"
#include "GradientComponent.h"
#include "Transform.h"
#include "ShipModule.h"
#include "ShipHighlight.h"

ModuleHighlightSystem::ModuleHighlightSystem()
	: EntitySystem(SystemType::ModuleHighlightSystem, 1,
	ComponentType::ShipModule)
{
}

void ModuleHighlightSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(p_entities[i]->getComponent(ComponentType::ShipModule));
		ShipHighlight* highlight = static_cast<ShipHighlight*>(p_entities[i]->getComponent(ComponentType::ShipHighlight));
		if (!highlight)
		{
			highlight = new ShipHighlight();
			p_entities[i]->addComponent(ComponentType::ShipHighlight, highlight);
			p_entities[i]->applyComponentChanges();
			highlight->active = false;
		}

		if (module->m_parentEntity >= 0)
		{
			Entity* ship = m_world->getEntity(module->m_parentEntity);
			ShipHighlight* shipHighlight = static_cast<ShipHighlight*>(ship->getComponent(ComponentType::ShipHighlight));
			if (shipHighlight)
			{
				highlight->active = shipHighlight->active;
				highlight->color = shipHighlight->color;
			}
			else
			{
				highlight->active = false;
			}
		}
		else
		{
			highlight->active = false;
		}
	}
}