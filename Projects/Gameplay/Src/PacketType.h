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
		// Server <--> Client:
		Ping,
		Pong,
		ShipTransform,

		// Server -> Client:
		EntityCreation,
		EntityUpdate,
		EntityDeletion,
		ClientDisconnect,
		WelcomePacket,
		UpdateClientStats,
		SpawnSoundEffect,
		TransformPacket,

		// Client -> Server:
		/************************************************************************/
		/* Client handles input and updates position, then sends it to the		*/
		/* server for verification.												*/
		/************************************************************************/
		ThrustPacket,
		RayPacket,
		ModuleHighlightPacket,
		SimpleEvent,
		ParticleUpdate,
	};

};