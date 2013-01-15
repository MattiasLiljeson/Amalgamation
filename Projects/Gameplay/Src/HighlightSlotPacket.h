#pragma once

#include "Packetizer.h"
#include "EntityType.h"

// =======================================================================================
//                                      HighlightSlotPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief
///        
/// # EntityUpdatePacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class HighlightSlotPacket : public Packetizer
{
public:
	HighlightSlotPacket();
	~HighlightSlotPacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	int id;

};