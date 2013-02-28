#pragma once

#include "Packetizer.h"
#include <AglVector3.h>

// =======================================================================================
//                                 ModuleStatusEffectPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A packet that can be sent to clients to use for visualizing various module
/// statuses.
///        
/// # ModuleStatusEffectPacket
/// Detailed description.....
/// Created on: 12-2-2013 
///---------------------------------------------------------------------------------------

class ModuleStatusEffectPacket : Packetizer
{
public:
	enum StatusType
	{
		UNUSEDMODULE_STATUS,	///< Effect if module is unused
		HEALTH_STATUS,			///< Effect based on module health
		VALUE_STATUS			///< Effect based on module value
	};
	enum Mode
	{
		ON,
		OFF
	};
	ModuleStatusEffectPacket();
	ModuleStatusEffectPacket(StatusType p_statusType, Mode p_mode, int p_networkId);
	ModuleStatusEffectPacket(StatusType p_statusType, float p_value, int p_networkId);
	~ModuleStatusEffectPacket();
	Packet pack();
	void unpack( Packet& p_packet );
public:
	int m_statusType;
	int m_mode;
	int m_moduleNetworkId;
	float m_value; ///< generic value for various effects.
};