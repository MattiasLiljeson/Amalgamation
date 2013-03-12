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
	enum Mode
	{
		HIGHLIGHT_SLOT,
		UNHIGHLIGHT_SLOT,
		UNHIGHLIGHT_ALL
	};
	HighlightSlotPacket();
	HighlightSlotPacket(Mode p_mode,int p_slot=0);
	~HighlightSlotPacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int id; ///< id of slot, -1 indicates to deactivate all slots
	int status; ///< On(1)/off(0)
};