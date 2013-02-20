#pragma once

#include "EntityType.h"
#include "Packetizer.h"
#include <AglVector3.h>

struct SpawnExplosionPacket : public Packetizer
{
public:
	SpawnExplosionPacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	AglVector3 position;
};