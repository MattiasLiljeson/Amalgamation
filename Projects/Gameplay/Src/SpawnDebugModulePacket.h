#pragma once
#include "Packetizer.h"
// =======================================================================================
// SpawnDebugModulePacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SpawnDebugModulePacket
/// Detailed description...
/// Created on: 14-3-2013 
///---------------------------------------------------------------------------------------
class SpawnDebugModulePacket: public Packetizer
{
public:
	SpawnDebugModulePacket();
	Packet pack() final;
	void unpack( Packet& p_packet ) final;

public:
	int moduleType;
};