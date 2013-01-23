#include "HudSystem.h"

#include "HudElement.h"
#include "LibRocketBackendSystem.h"
#include <DebugUtil.h>

HudSystem::HudSystem( LibRocketBackendSystem* p_backend )
	: EntitySystem( SystemType::HudSystem, 1, ComponentType::HudElement )
{
	m_backend = p_backend;
}


HudSystem::~HudSystem()
{
}

void HudSystem::initialize()
{

}

void HudSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++)
	{
		HudElement* hudElement = static_cast<HudElement*>(
				p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::HudElement ) );
		
		if( hudElement->hasChanged() )
		{
			//string print = hudElement->getValue() + "\n";
			//DEBUGPRINT((print.c_str()));

			m_backend->updateElement(0, hudElement->getElement(), hudElement->getValue());
			
			hudElement->setRead();
		}
	}
}
