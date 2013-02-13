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
	m_hudIndex = m_backend->loadDocument( GUI_HUD_PATH.c_str(), "hud" );
	m_backend->showDocument(m_hudIndex);

	setHUDData(SCORE,"0");
	setHUDData(MAPPING,"MAAPPPING");
	setHUDData(TIME,"10:00");
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

void HudSystem::setHUDData( HUD_TYPES p_type, const char* p_value )
{
	switch (p_type)
	{
	case HudSystem::TIME:
		m_backend->updateElement(m_hudIndex,TIMERELEMENT,p_value);
		break;
	case HudSystem::SCORE:
		m_backend->updateElement(m_hudIndex,SCOREELEMENT,p_value);
		break;
	case HudSystem::MAPPING:
		m_backend->updateElement(m_hudIndex,MAPPINGELEMENT,p_value);
		break;
	default:
		break;
	}
}
