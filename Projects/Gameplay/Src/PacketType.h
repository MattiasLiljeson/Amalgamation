// =======================================================================================
//	PacketType
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PacketType
/// Detailed description...
/// Created on: 12-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class PacketType
{
public:
	enum
	{
		NON_EXISTENT = -1,
		// Server -><- Client:
		Ping,
		Pong,
		// Server -> Client:
		EntityCreation,
		EntityUpdate,
		ClientDisconnect,
		WelcomePacket,
		ScoresUpdate,
		// Client -> Server:
		PlayerInput,

		UpdateClientStats,
	};

};