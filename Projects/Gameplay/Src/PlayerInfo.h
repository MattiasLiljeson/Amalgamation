#pragma once
#include "Packetizer.h"

// =======================================================================================
//                                      PlayerInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the information needed by the server to setup playerInfo
///        
/// # PlayerInfo
/// Detailed description.....
/// Created on: 26-2-2013 
///---------------------------------------------------------------------------------------

class PlayerInfo : public Packetizer
{
public:
	PlayerInfo();
	~PlayerInfo();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	string playerName;
	int playerID;
};