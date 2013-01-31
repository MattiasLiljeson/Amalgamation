#include "ShipTransformPacket.h"

ShipTransformPacket::ShipTransformPacket()
{
}

Packet ShipTransformPacket::pack()
{
	Packet packet( (char)PacketType::ShipTransform );
	packet << transform;
	packet << entityId;

	return packet;
}

void ShipTransformPacket::unpack( Packet p_packet )
{
	p_packet >> transform;
	p_packet >> entityId;
}