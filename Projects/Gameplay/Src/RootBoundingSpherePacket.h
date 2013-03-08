#pragma once

#include "EntityType.h"
#include <AglVector3.h>
#include "Packetizer.h"

struct RootBoundingSpherePacket : public Packetizer
{
public:
	RootBoundingSpherePacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int targetNetworkIdentity;
	AglVector3 position;
	float radius;
};