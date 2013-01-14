#pragma once

#include <Packet.h>
#include "Packetizer.h"

// =======================================================================================
//	ThrustPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # ThrustPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ThrustPacket: Packetizer
{
public:
	ThrustPacket();

	///-----------------------------------------------------------------------------------
	/// Generate a packet from the ThrustPacket's variables.
	/// \return Packet
	///-----------------------------------------------------------------------------------
	Packet pack();

	///-----------------------------------------------------------------------------------
	/// Unpacks a packet, storing it in the ThrustPacket's variables.
	/// \param p_packet
	/// \return void
	///-----------------------------------------------------------------------------------
	void unpack( Packet p_packet );

public:
	int entityId;
	AglVector3 thrustVector;
	AglVector3 angularVector;

};