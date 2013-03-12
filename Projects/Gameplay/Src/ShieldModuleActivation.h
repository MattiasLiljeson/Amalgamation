#pragma once
#include "ModuleEvent.h"
#include <Entity.h>
#include "Transform.h"
#include <TcpServer.h>
#include "ModuleTriggerPacket.h"
//#include "SpawnSoundEffectPacket.h"
//#include "RemoveSoundEffectPacket.h"

// =======================================================================================
// SpeedBoostModuleActivation
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SpeedBoostModuleActivation
/// Detailed description...
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------
class ShieldModuleActivation: public ModuleEvent
{
public:
	ShieldModuleActivation(Entity* p_shipEntity, TcpServer* p_server)
	{
		m_shipEntity = p_shipEntity;
		m_server = p_server;
	}
	
	void activate()
	{
		Transform* transform = static_cast<Transform*>(m_shipEntity->getComponent(
			ComponentType::Transform));
		if(transform)
		{
			ModuleTriggerPacket data;
			data.moduleNetsyncIdentity = m_shipEntity->getIndex();
			data.moduleTrigger = true;
			m_server->broadcastPacket(data.pack());
		}
	}

	void deactivate()
	{
		Transform* transform = static_cast<Transform*>(m_shipEntity->getComponent(
			ComponentType::Transform));
		if(transform)
		{
			ModuleTriggerPacket data;
			data.moduleNetsyncIdentity = m_shipEntity->getIndex();
			data.moduleTrigger = false;
			m_server->broadcastPacket(data.pack());
		}
	}

private:
	Entity* m_shipEntity;
	TcpServer* m_server;
};