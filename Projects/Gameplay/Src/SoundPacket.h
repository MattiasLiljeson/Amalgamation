#pragma once

#include "Packetizer.h"
#include <AudioHeader.h>

struct SoundPacket: public Packetizer
{
public:
	SoundPacket();

	virtual Packet pack();

	virtual void unpack( Packet& p_packet );

public:
	int			queuedPlayingState;
	int			soundType;
	int			target;
	int			targetSlot;
};