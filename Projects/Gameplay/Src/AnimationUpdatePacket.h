#pragma once

#include "EntityType.h"
#include <AglVector3.h>
#include <AglQuaternion.h>
#include "Packetizer.h"

// =======================================================================================
//                                      AnimationUpdatePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of animation updates.
///        
/// # EntityUpdatePacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

struct AnimationUpdatePacket : public Packetizer
{
public:
	AnimationUpdatePacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int networkIdentity;
};