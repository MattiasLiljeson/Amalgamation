#pragma once
#include "Packetizer.h"
// =======================================================================================
//                                      NewlyConnectedPlayerPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NewlyConnectedPlayerPacket
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class NewlyConnectedPlayerPacket: public Packetizer
{
public:
	NewlyConnectedPlayerPacket();
	Packet pack();
	void unpack( Packet& p_packet );
	
public:
	int		playerID;
	string	playerName;
	int		networkID;
	float	score;
	int		ping;
};