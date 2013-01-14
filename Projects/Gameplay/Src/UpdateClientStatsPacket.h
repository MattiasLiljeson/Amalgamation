#pragma once
#include "Packetizer.h"

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
	float ping;
	int score[8];
};