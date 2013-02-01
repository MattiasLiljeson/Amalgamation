#pragma once
#include "ModuleEvent.h"
#include "PositionalSoundSource.h"
#include <Entity.h>
#include "Transform.h"
#include <TcpServer.h>
// =======================================================================================
// OnActivateShieldModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # OnActivateShieldModule
/// Detailed description...
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------
class OnActivateShieldModule: public ModuleEvent
{
public:
	OnActivateShieldModule(Entity* p_shipEntity, TcpServer* p_server)
	{
		m_shipEntity = p_shipEntity;
		m_server = p_server;
	}
	
	void happen() // NOTE: (Johan) try "virtual void happen() final" later. :)
	{
		Transform* transform = static_cast<Transform*>(m_shipEntity->getComponent(
			ComponentType::Transform));
		if(transform)
		{
			SpawnSoundEffectPacket data;
			data.attachedToNetsyncEntity = m_shipEntity->getIndex();
			data.positional = true;
			data.position = transform->getTranslation();
			data.soundIdentifier = SpawnSoundEffectPacket::MineBlip;
			m_server->broadcastPacket(data.pack());
		}
	}

private:
	Entity* m_shipEntity;
	TcpServer* m_server;
};