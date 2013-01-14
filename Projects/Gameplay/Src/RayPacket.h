#pragma once

#include "Packetizer.h"
#include <AglVector3.h>

// =======================================================================================
//	RayPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # RayPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class RayPacket: Packetizer
{
public:
	RayPacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	AglVector3 o;
	AglVector3 d;

};