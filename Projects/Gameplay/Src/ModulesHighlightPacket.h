#pragma once
#include "Packetizer.h"
// =======================================================================================
// ModulesHighlightPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief An array of netsync identities of modules that are highlighted.
///        
/// # ModulesHighlightPacket
/// Detailed description...
/// Created on: 11-3-2013 
///---------------------------------------------------------------------------------------
class ModulesHighlightPacket: public Packetizer
{
public:
	ModulesHighlightPacket();
	void unpack( Packet& p_packet ) final;
	Packet pack() final;

public:
	static const int NUM_MODULES_HIGHLIGHTED_MAX = 32;

	unsigned char numberOfHighlights;
	int modulesHighighted[NUM_MODULES_HIGHLIGHTED_MAX];
};