#pragma once

#include "Packetizer.h"
#include <AglMatrix.h>

// =======================================================================================
//	ShipTransformPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief ShipTransform information to send to client or server
///        
/// # ShipTransformPacket
/// Detailed description...
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ShipTransformPacket: Packetizer
{
public:
	ShipTransformPacket();

	Packet pack();

	void unpack( Packet p_packet );

public:
	AglMatrix transform;
	int entityId;
};