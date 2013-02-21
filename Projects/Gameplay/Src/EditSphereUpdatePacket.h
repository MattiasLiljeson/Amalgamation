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

struct EditSphereUpdatePacket : public Packetizer
{
public:
	EditSphereUpdatePacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	AglVector3 m_offset;
	float m_radius;
};