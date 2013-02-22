#include "HudSystem.h"

#include "HudElement.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>
#include "GameState.h"

HudSystem::HudSystem( LibRocketBackendSystem* p_backend )
	: EntitySystem( SystemType::HudSystem, 1, ComponentType::GameState )
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

}

void HudSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(p_entities.size()>0){
		auto comp = static_cast<GameState*>(p_entities[0]->getComponent(
			ComponentType::GameState));

		if(comp->getStateDelta(INGAME) != 0){
			m_backend->showDocument(m_hudIndex);

			// setHUDData(SCORE,"-1");
			setHUDData(MAPPING,"Empty");
			setHUDData(TIME,"00:00");
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
