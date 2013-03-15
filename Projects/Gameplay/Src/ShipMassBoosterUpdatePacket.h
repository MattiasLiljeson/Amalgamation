#pragma once

#include "Packetizer.h"

// =======================================================================================
//                                      ShipMassUpdatePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShipMassUpdatePacket
/// Detailed description.....
/// Created on: 15-3-2013 
///---------------------------------------------------------------------------------------

class ShipMassBoosterUpdatePacket
{
public:
	ShipMassBoosterUpdatePacket();
	virtual ~ShipMassBoosterUpdatePacket();

	Packet pack();

	void unpack( Packet& p_packet );

	float mass;
	int boosters;
protected:
private:
};