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

		// Server -> Client:
		EntityCreation,
		EntityUpdate,
		EntityDeletion,
		ClientDisconnect,
		WelcomePacket,
		UpdateClientStats,
		PlayerWinLose,
		SpawnSoundEffect,

		// Client -> Server:
		/************************************************************************/
		/* Client handles input and updates position, then sends it to the		*/
		/* server for verification.												*/
		/************************************************************************/
		ShipTransform,
		ThrustPacket,
		RayPacket,
		ModuleHighlightPacket,
		SimpleEvent,
		ParticleUpdate,
		ParticleSystemCreationInfo
	};

};