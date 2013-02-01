#pragma once
#include "Packetizer.h"
// =======================================================================================
// RemoveSoundEffectPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # RemoveSoundEffectPacket
/// Detailed description...
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------
class RemoveSoundEffectPacket: public Packetizer
{
public:
	RemoveSoundEffectPacket();
	Packet pack();
	void unpack( Packet p_packet );

public:
	int attachedNetsyncIdentity;
};