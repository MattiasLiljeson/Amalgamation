#pragma once

#include "Packetizer.h"

// =======================================================================================
//                                      EntityDeletionPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles packing an unpacking of delete entity packets
///        
/// # EntityDeletionPacket
/// Detailed description.....
/// Created on: 28-1-2013 
///---------------------------------------------------------------------------------------

class EntityDeletionPacket : public Packetizer
{
public:
	EntityDeletionPacket();
	virtual ~EntityDeletionPacket();

	virtual Packet pack();

	virtual void unpack( Packet p_packet );

public:
	int networkIdentity;
};