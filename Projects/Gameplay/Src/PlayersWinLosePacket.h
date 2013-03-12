#pragma once
#include "Packetizer.h"
#include <Globals.h>

// =======================================================================================
//	PlayersWinLosePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PlayersWinLosePacket
/// Detailed description...
/// Created on: 30-1-2013 
///---------------------------------------------------------------------------------------

class PlayersWinLosePacket : public Packetizer
{
public:
	PlayersWinLosePacket();
	Packet pack();
	void unpack( Packet& p_packet );

public:
	int activePlayers;
	int playerIdentities[MAXPLAYERS];
	float scores[MAXPLAYERS];
	bool winner[MAXPLAYERS];
};