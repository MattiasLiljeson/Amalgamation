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
		ChangeStatePacket, 
		PlayerReadyPacket,
		PlayerInfo,

		// Server -> Client:
		EntityCreation,
		EntityUpdate,
		EntityDeletion,
		ClientDisconnect,
		WelcomePacket,
		NewlyConnectedPlayerPacket,
		UpdateClientStats,
		PlayerWinLose,
		SpawnSoundEffect, // NOTE: (Johan) Should rather be replaced than be used more. (Move sound to client only)
		ModuleTriggerPacket,
		TransformPacket,
		RemoveSoundEffect,
		OnHitScoreEffectPacket,
		ModuleStateChangePacket,
		ModuleStatusEffectPacket,
		BombActivationPacket,
		TeslaHitPacket,
		HitIndicatorPacket,
		ModulesHighlightPacket,
		ShieldActivationPacket,

		// Client -> Server:
		/************************************************************************/
		/* Client handles input and updates position, then sends it to the		*/
		/* server for verification.												*/
		/************************************************************************/
		ThrustPacket,
		RayPacket,
		HighlightSlotPacket,
		SimpleEvent,
		ParticleUpdate,
		ParticleSystemCreationInfo,
		CameraControlPacket,
		AnimationUpdatePacket,
		SlotParticleEffectPacket,
		SpawnExplosionPacket,
		EditSphereUpdatePacket,
		SelectionMarkerUpdatePacket,
		HighlightEntityPacket,
		RootBoundingSpherePacket,
		SoundPacket,
		SpawnDebugModulePacket,
	};

};