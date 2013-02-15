#pragma once

#include "Packetizer.h"
#include <AglVector3.h>

// =======================================================================================
//                                  OnHitScoreEffectPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A packet that can be sent to clients to use for visualizing various hit effects
/// such as score numbers or ricocheting bullets.
/// Sent to the client whose score is affected.
///        
/// # OnHitScoreEffectPacket
/// Detailed description.....
/// Created on: 12-2-2013 
///---------------------------------------------------------------------------------------

class OnHitScoreEffectPacket : Packetizer
{
public:
	OnHitScoreEffectPacket();
	~OnHitScoreEffectPacket();
	Packet pack();
	void unpack( Packet& p_packet );
public:
	AglVector3 position;
	AglQuaternion angle;
	int score;
};