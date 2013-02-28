#pragma once

#include "Packetizer.h"

class HighlightEntityPacket: Packetizer
{
public:
	HighlightEntityPacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int		target;
	bool	on;

};