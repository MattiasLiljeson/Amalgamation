#include "HudSystem.h"

#include "HudElement.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>

HudSystem::HudSystem( LibRocketBackendSystem* p_backend )
	: EntitySystem( SystemType::HudSystem, 1, ComponentType::HudElement )
{
	m_backend = p_backend;
	m_hudVisible = false;
	m_currentState = false;
}


HudSystem::~HudSystem()
{
}

void HudSystem::initialize()
{
	int i = m_backend->loadDocument( GUI_HUD_PATH.c_str(), "hud" );
	m_backend->showDocument(i);
}

void HudSystem::processEntities( const vector<Entity*>& p_entities )
{
	bool changed = false;
	if(m_currentState != m_hudVisible){
		m_currentState = m_hudVisible;
		changed = true;
	}

	for(unsigned int i=0;i < p_entities.size(); i++){

		if(changed){
		}
	}
}

void HudSystem::setHUDVisebilty(bool p_setVisibility)
{
	m_hudVisible = p_setVisibility;
}
