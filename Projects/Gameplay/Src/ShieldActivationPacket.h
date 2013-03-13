#pragma once
#include "Packetizer.h"
// =======================================================================================
// ShieldActivationPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Contains the activation state of a shield module.
///        
/// # ShieldActivationPacket
/// Detailed description...
/// Created on: 13-3-2013 
///---------------------------------------------------------------------------------------
class ShieldActivationPacket: public Packetizer
{
public:
	ShieldActivationPacket();
	Packet pack() final;
	void unpack( Packet& p_packet ) final;


public:
	bool shieldActivationState;
	int entityIndex;
};