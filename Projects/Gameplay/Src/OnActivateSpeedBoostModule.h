#pragma once
#include <TcpServer.h>
#include <Entity.h>
#include "Transform.h"
#include "SpawnSoundEffectPacket.h"
// =======================================================================================
// OnActivateSpeedBoostModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # OnActivateSpeedBoostModule
/// Detailed description...
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
class OnActivateSpeedBoostModule: public ModuleEvent
{
public:
	OnActivateSpeedBoostModule(Entity* p_shipEntity, TcpServer* p_server)
	{
		m_shipEntity = p_shipEntity;
		m_server = p_server;
	}
	
	void happen()
	{
		Transform* transform = static_cast<Transform*>(m_shipEntity->getComponent(
			ComponentType::Transform));
		if(transform)
		{
			SpawnSoundEffectPacket data;
			data.attachedToNetsyncEntity = m_shipEntity->getIndex();
			data.positional = true;
			data.position = transform->getTranslation();
			data.soundIdentifier = SpawnSoundEffectPacket::SpeedBoostActive;
			m_server->broadcastPacket(data.pack());
		}
	}

private:
	Entity* m_shipEntity;
	TcpServer* m_server;
};