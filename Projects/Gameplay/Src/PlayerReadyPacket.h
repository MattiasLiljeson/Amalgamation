#pragma once

#include "Packetizer.h"

// =======================================================================================
//                                      PlayerReadyPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # PlayerReadyPacket
/// Detailed description.....
/// Created on: 11-3-2013 
///---------------------------------------------------------------------------------------

class PlayerReadyPacket : public Packetizer
{
public:
	PlayerReadyPacket();
	~PlayerReadyPacket();

	Packet pack();
	void unpack( Packet& p_packet );

public:
	int		playerId;
	bool	ready;
};