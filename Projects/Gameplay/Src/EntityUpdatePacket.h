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

class EntityUpdatePacket : public Packetizer
{
public:
	EntityUpdatePacket();
	~EntityUpdatePacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	/************************************************************************/
	/* EntityType Might not be needed as the networkIdentity can fetch the	*/
	/* type?																*/
	/************************************************************************/
	char entityType; 
	int networkIdentity;
	AglVector3		translation;
	AglVector3		scale;
	AglQuaternion	rotation;
	float timestamp;
	AglVector3 velocity;
	AglVector3 angularVelocity;
};