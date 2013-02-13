#pragma once
#include "Packetizer.h"
// =======================================================================================
// ModuleTriggerPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ModuleTriggerPacket
/// Detailed description...
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------
class ModuleTriggerPacket: public Packetizer
{
public:
	ModuleTriggerPacket();
	virtual Packet pack() final;
	virtual void unpack( Packet& p_packet ) final;

public:
	int moduleNetsyncIdentity;
	bool moduleTrigger;

};