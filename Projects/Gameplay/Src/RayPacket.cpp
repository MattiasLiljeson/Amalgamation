#include "RayPacket.h"
#include "PacketType.h"

RayPacket::RayPacket()
{
}

Packet RayPacket::pack()
{
	Packet packet( (char)PacketType::RayPacket );
	packet << o;
	packet << d;

	return packet;
}

void RayPacket::unpack( Packet& p_packet )
{
	p_packet >> o;
	p_packet >> d;
}