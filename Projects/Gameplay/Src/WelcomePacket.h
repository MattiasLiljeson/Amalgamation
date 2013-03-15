#pragma once
#include "Packetizer.h"

// =======================================================================================
//                                      WelcomePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of WelcomePacket
///        
/// # WelcomePacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class WelcomePacket : public Packetizer
{
public:
	WelcomePacket();
	 ~WelcomePacket();

	Packet pack();

	void unpack( Packet& p_packet );
public:
	int clientNetworkIdentity;
	int playerID;
	int serverGameTime;
	string serverName;
};