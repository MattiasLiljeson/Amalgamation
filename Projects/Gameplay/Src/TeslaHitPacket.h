#pragma once
#include "Packetizer.h"
// =======================================================================================
// TeslaHitPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # TeslaHitPacket
/// Detailed description...
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------
class TeslaHitPacket: public Packetizer
{
public:
	TeslaHitPacket();

	Packet pack() final;
	void unpack(Packet& p_packet) final;

public:
	static const int NUM_TESLA_HITS_MAX = 10;

	unsigned char numberOfHits;
	int identitiesHit[NUM_TESLA_HITS_MAX];

};