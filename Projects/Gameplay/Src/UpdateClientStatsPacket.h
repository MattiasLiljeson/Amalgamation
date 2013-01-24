#pragma once
#include "Packetizer.h"
#include <Globals.h>

// =======================================================================================
//                                      UpdateClientStatsPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of UpdateClientsPacket
///        
/// # UpdateClientStatsPacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class UpdateClientStatsPacket : public Packetizer
{
public:
	UpdateClientStatsPacket();
	~UpdateClientStatsPacket();

	Packet pack();

	void unpack( Packet p_packet );
public:
	int	activePlayers;
	float ping[MAXPLAYERS];
	float currentServerTimestamp;
	int playerIdentities[MAXPLAYERS];
	float scores[MAXPLAYERS];
};