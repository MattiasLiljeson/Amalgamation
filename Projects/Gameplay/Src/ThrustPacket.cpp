#include "ThrustPacket.h"
#include "PacketType.h"

ThrustPacket::ThrustPacket()
{
	entityId = -1;
	thrustVector = AglVector3();
	angularVector = AglVector3();
}

Packet ThrustPacket::pack()
{
	Packet packet( (char)PacketType::ThrustPacket );

	packet << entityId;
	packet << thrustVector;
	packet << angularVector;

	return packet;
}

void ThrustPacket::unpack( Packet& p_packet )
{
	p_packet >> entityId;
	p_packet >> thrustVector;
	p_packet >> angularVector;
}
