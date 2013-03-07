#pragma once
#include "Packetizer.h"

// =======================================================================================
//									DisconnectPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of DisconnectPacket
///        
/// # WelcomePacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class DisconnectPacket : public Packetizer
{
public:
	DisconnectPacket();
	~DisconnectPacket();

	Packet pack();

	void unpack( Packet& p_packet );
public:
	int clientNetworkIdentity;
	int playerID;
};