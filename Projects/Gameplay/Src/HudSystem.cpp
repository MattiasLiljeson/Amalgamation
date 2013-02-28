#include "HudSystem.h"

#include "HudElement.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>
#include "ClientStateSystem.h"

HudSystem::HudSystem( LibRocketBackendSystem* p_backend )
	: EntitySystem( SystemType::HudSystem )
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

void HudSystem::process()
{
	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));
	
	if(stateSystem->getStateDelta(GameStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME)
	{
		m_backend->showDocument(m_hudIndex);

		setHUDData(SCORE,"-1");
		//setHUDData(MAPPING,"Empty");
		setHUDData(TIME,"00:00");
	}
	if(stateSystem->getStateDelta(GameStates::RESULTS) == EnumGameDelta::ENTEREDTHISFRAME){
		m_backend->hideDocument(m_hudIndex);
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
