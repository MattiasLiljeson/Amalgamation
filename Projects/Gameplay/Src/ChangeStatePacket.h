#pragma once
#include "Packetizer.h"
#include "GameState.h"
// =======================================================================================
//                                      ChangeStatePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Allows for state changing requests over the network
///        
/// # ChangeStatePacket
/// Detailed description.....
/// Created on: 22-2-2013 
///---------------------------------------------------------------------------------------

class ChangeStatePacket : public Packetizer
{
public:
	ChangeStatePacket(){
		m_gameState = GameStates::NONE;
		m_serverState = ServerStates::NONE;
	}
	~ChangeStatePacket(){

	}

	Packet pack() 
	{
		Packet packet(PacketType::ChangeStatePacket);
		packet << (int) m_gameState << (int)m_serverState;

		return packet;
	}

	void unpack( Packet& p_packet ) 
	{
		int gameState, serverState;
		p_packet >> gameState >> serverState;

		m_gameState = static_cast<GameStates>(gameState);
		m_serverState = static_cast<ServerStates>(serverState);
	}
public:
	GameStates m_gameState;
	ServerStates m_serverState;
};