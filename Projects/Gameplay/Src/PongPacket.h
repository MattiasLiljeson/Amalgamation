#pragma once

#include "Packetizer.h"

// =======================================================================================
//	PongPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PongPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class PongPacket: public Packetizer
{
public:
	PongPacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	float timeStamp;

};