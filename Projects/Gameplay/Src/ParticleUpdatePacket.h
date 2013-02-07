#pragma once

#include "Packetizer.h"
#include "EntityType.h"

// =======================================================================================
//                                      EntityUpdatePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of entity updates.
///        
/// # EntityUpdatePacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ParticleUpdatePacket : public Packetizer
{
public:
	ParticleUpdatePacket();
	~ParticleUpdatePacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int networkIdentity;
	AglVector3 position;
	AglVector3 direction;
	float	   speed;
	float	   spawnFrequency;
};