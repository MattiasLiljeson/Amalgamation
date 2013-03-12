#pragma once

#include "Packetizer.h"

// =======================================================================================
//	PingPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PingPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class PingPacket: Packetizer
{
public:
	PingPacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	float timeStamp;

};