#include "PingPacket.h"
#include "PacketType.h"

PingPacket::PingPacket()
{
	clientTime = 0;
}

Packet PingPacket::pack()
{
	Packet packet( PacketType::Ping );
	packet << clientTime;

	return packet;
}

void PingPacket::unpack( Packet p_packet )
{
	p_packet >> clientTime;
}