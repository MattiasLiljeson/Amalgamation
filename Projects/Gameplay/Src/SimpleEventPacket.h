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
	ACTIVATE_PICK, //Starts picking an object
	DEACTIVATE_PICK, //Stops picking an object. Object snaps to closest location
	RELEASE_PICK, //Stops picking an object. Objects is released back into space
	ROTATE_ADD,
	ROTATE_SUB,
	ROTATE_NONE,
	EVENT_NONE,
};

class SimpleEventPacket: Packetizer
{
public:
	SimpleEventPacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int type;

};