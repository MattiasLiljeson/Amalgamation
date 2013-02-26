#pragma once

#include "EntityType.h"
#include <AglVector3.h>
#include <AglQuaternion.h>
#include "Packetizer.h"

struct SelectionMarkerUpdatePacket : public Packetizer
{
public:
	SelectionMarkerUpdatePacket();

	Packet pack();

	void unpack( Packet& p_packet );

public:
	int targetNetworkIdentity;
	AglMatrix transform;
};