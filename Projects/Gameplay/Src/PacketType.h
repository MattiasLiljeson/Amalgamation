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

		/************************************************************************/
		/* Sends back the location for the ship. Can be approved or not			*/
		/************************************************************************/
		ShipLocationResponse, 
		// Client -> Server:
		PlayerInput,

		/************************************************************************/
		/* Client handles input and updates position, then sends it to the		*/
		/* server for verification.												*/
		/************************************************************************/
		ShipTransform,

		UpdateClientStats,
	};

};