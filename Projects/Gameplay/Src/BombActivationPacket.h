#pragma once
#include "Packetizer.h"
// =======================================================================================
// BombActivationPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # BombActivationPacket
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class BombActivationPacket: public Packetizer
{
public:
	BombActivationPacket();
	virtual Packet pack() final;
	virtual void unpack( Packet& p_packet ) final;

public:
	int netsyncId;

};