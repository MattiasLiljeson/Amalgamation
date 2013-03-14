#pragma once
#include "Packetizer.h"
#include <AglVector3.h>
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
	static const int MAX_MODULETYPES = 10;

	AglVector3 shipPosition;
	unsigned char numberOfModules;
	int moduleTypes[MAX_MODULETYPES];
};