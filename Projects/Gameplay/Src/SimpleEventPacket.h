#pragma once

#include "Packetizer.h"

// =======================================================================================
//	SimpleEventPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # SimpleEventPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

enum SimpleEventType
{
	ACTIVATE_MODULE,
	DEACTIVATE_MODULE,
	EVENT_NONE,
};

class SimpleEventPacket: Packetizer
{
public:
	SimpleEventPacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	int type;

};