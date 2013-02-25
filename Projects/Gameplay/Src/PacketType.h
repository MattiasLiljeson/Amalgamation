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
		SpawnSoundEffect, // NOTE: (Johan) Should rather be replaced than be used more. (Move sound to client only)
		ModuleTriggerPacket,
		TransformPacket,
		RemoveSoundEffect,
		OnHitScoreEffectPacket,
		ModuleStateChangePacket,

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
		ParticleSystemCreationInfo,
		CameraControlPacket,
		AnimationUpdatePacket,
		SlotParticleEffectPacket,
		SpawnExplosionPacket,
		EditSphereUpdatePacket,
		SelectionMarkerUpdatePacket,
		HighlightEntityPacket
	};

};