#pragma once

#include "PacketType.h"
#include <Packet.h>

// =======================================================================================
//	Packetizer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # Packetizer
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class Packetizer
{
public:
	virtual Packet pack() = 0;
	virtual void unpack( Packet& p_packet ) = 0;

};