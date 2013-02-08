#pragma once

#include <Packet.h>
#include "Packetizer.h"
#include <PlayerStates.h>

// =======================================================================================
//                                      CameraControlPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Packet describing various client actions regarding the camera. 
/// These will most likely be state changes between steer- and edit mode, 
/// as well as manual control during edit mode.
///        
/// # CameraControlPacket
/// Detailed description.....
/// Created on: 5-2-2013 
///---------------------------------------------------------------------------------------

class CameraControlPacket : public Packetizer
{
public:
	CameraControlPacket();
	virtual ~CameraControlPacket();

	///-----------------------------------------------------------------------------------
	/// Generate a packet from the CameraControlPacket's variables.
	/// \return Packet
	///-----------------------------------------------------------------------------------
	Packet pack();

	///-----------------------------------------------------------------------------------
	/// Unpacks a packet, storing it in the CameraControlPacket's variables.
	/// \param p_packet
	/// \return void
	///-----------------------------------------------------------------------------------
	void unpack( Packet& p_packet );

	AglVector3 movement;
	int state;
	int entityId;
};